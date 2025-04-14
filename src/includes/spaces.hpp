#pragma once

#include "worldbase.hpp"
#include "vector2d.hpp"
#include "affinetransform.hpp"
#include "node.hpp"

namespace Core
{
    Vector2D tViewPoint_{};
    AffineTransform ntwt_{true};
    AffineTransform afft_{true};

    /// @brief mapDeviceToView maps mouse-space device coordinates to view-space
    /// @param world WorldBase
    /// @param dvx
    /// @param dvy
    /// @param viewPoint Vector2D
    static void mapDeviceToView(
        const Worldbase &world,
        double dvx,
        double dvy,
        Vector2D &viewPoint)
    {
        viewPoint.set(dvx, dvy);

        viewPoint.mul(world.invViewSpace);
    }

    /// @brief
    /// mapDeviceToNode maps mouse-space device coordinates to local node-space
    /// Optional scaling before returning from function. <b>Extremely</b> rare to use:
    /// ```
    ///     localPoint.SetByComp(localPoint.X() * node.Scale(), localPoint.Y() * node.Scale())
    /// ```
    /// @param world
    /// @param dvx
    /// @param dvy
    /// @param node
    /// @param localPoint
    static void mapDeviceToNode(
        const Worldbase &world, int dvx, int dvy, nodeShPtr node, Vector2D &localPoint)
    {
        // Mapping from device to node requires transforms from two "directions"
        // 1st is upwards transform and the 2nd is downwards transform.

        // downwards from device-space to view-space
        // if (world.deviceSize != null) {
        //   dvy = world.deviceSize!.height.toInt() - dvy;
        //   print(dvy);
        // }
        mapDeviceToView(world, dvx, dvy, tViewPoint_);

        // Canvas and OpenGL have inverted Y axis. Canvas is +Y downward.
        // OpenGL's +Y axis is upwards so we either flip the Y axis here
        // or flip the mouse's +Y axis in cursorPosCallback(...)
        // dvr := node.World().Properties().Window.DeviceRes
        // MapDeviceToView(node.World(), dvx, int32(dvr.Height)-dvy, tViewPoint)

        // Upwards from node to world-space (aka view-space)
        afft_.toIdentity();
        worldToNodeTransform(node, nullptr, afft_);

        // Now map view-space point to local-space of node
        afft_.transform(tViewPoint_.x, tViewPoint_.y, localPoint);
    }

    /// @brief
    /// mapNodeToNode maps node's local origin (0,0) to another node's space
    /// Supplying a psuedo-root can reduce multiplications, thus use it if possible.
    /// @param sourceNode
    /// @param destinationNode
    /// @param nodePoint
    /// @param psuedoRoot
    static void mapNodeToNode(
        nodeShPtr sourceNode, nodeShPtr destinationNode, Vector2D &nodePoint, nodeShPtr psuedoRoot)
    {
        afft_.toIdentity();
        nodeToWorldTransform(sourceNode, psuedoRoot, afft_);
        afft_.transform(0.0, 0.0, nodePoint);
        // nodePoint is now in world-space

        afft_.toIdentity();
        worldToNodeTransform(destinationNode, psuedoRoot, afft_); // nodePoint
        afft_.transform(nodePoint.x, nodePoint.y, nodePoint);
        // nodePoint is now in the destination node's space

        // OR via view-space (not recommended as it is extra operations and rounding)
        // MapNodeToDevice(sourceNode.World(), sourceNode, nodePoint)
        // MapDeviceToNode(int32(nodePoint.X()), int32(nodePoint.Y()), destinationNode, nodePoint)
    }

    /// @brief
    /// mapNodeToWorld maps node's local origin to world-space
    /// @param node
    /// @param point
    static void mapNodeToWorld(nodeShPtr node, Vector2D &point)
    {
        afft_.toIdentity();
        nodeToWorldTransform(node, nullptr, afft_);
        afft_.transform(0.0, 0.0, point);
    }

    /// @brief
    /// mapWorldToNode maps a world-space coord to a specific node
    /// @param node
    /// @param worldPoint
    /// @param nodePoint
    static void mapWorldToNode(nodeShPtr node, const Vector2D &worldPoint, Vector2D &nodePoint)
    {
        afft_.toIdentity();
        worldToNodeTransform(node, nullptr, afft_); // nodePoint
        afft_.transform(worldPoint.x, worldPoint.y, nodePoint);
    }

    /// @brief
    /// mapNodeToDevice maps node local origin to device-space (aka mouse-space)
    /// @param world
    /// @param node
    /// @param devicePoint
    static void mapNodeToDevice(const Worldbase &world, nodeShPtr node, Vector2D &devicePoint)
    {
        afft_.toIdentity();
        nodeToWorldTransform(node, nullptr, afft_);
        afft_.transform(0.0, 0.0, devicePoint);
        devicePoint.mul(world.viewSpace);
    }

    /// @brief
    /// worldToNodeTransform maps a world-space coordinate to local-space of node
    /// @param node
    /// @param
    /// @return
    static void worldToNodeTransform(nodeShPtr node, nodeShPtr psuedoRoot, AffineTransform &out)
    {
        nodeToWorldTransform(node, psuedoRoot, out);
        out.invert();
    }

    /// @brief
    /// nodeToWorldTransform maps a local-space coordinate to world-space
    /// @param node
    /// @param
    /// @return
    static void nodeToWorldTransform(nodeShPtr node, nodeShPtr psuedoRoot, AffineTransform &out)
    {
        // A transform to accumulate the parent transforms.
        ntwt_.set(node->calcTransform());

        // Iterate "upwards" starting with the child towards the parents
        // starting with this child's parent.
        auto p = node->parent;

        while (!p.expired())
        {
            if (nodeShPtr pShr = p.lock())
            {
                auto parentT = pShr->calcTransform();

                // Because we are iterating upwards we need to pre-multiply each child.
                // Ex: [child] x [parent]
                // ----------------------------------------------------------
                //           [ntwt_] x [parentT]
                //               |
                //               | out
                //               v
                //             [ntwt_] x [parentT]
                //                 |
                //                 | out
                //                 v
                //               [ntwt_] x [parentT...]
                //
                // This is a pre-multiply order
                // [child] x [parent of child] x [parent of parent of child]...
                //
                // In other words the child is mutiplied "into" the parent.
                AffineTransform::multiplyPost(ntwt_, *parentT, out);
                ntwt_.set(out); // copy 'out' into 'ntwt_' for next loop

                if (pShr == psuedoRoot)
                {
                    // fmt.Println("SpaceMappings: hit psuedoRoot")
                    break;
                }

                // Work our way upwards to the next parent
                p = pShr->parent;
            }
        }

        out.set(ntwt_);
    }
}