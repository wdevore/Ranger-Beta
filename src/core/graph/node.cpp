#include <iostream>

#include "node.hpp"

namespace Core
{

    Group &Node::getGroup()
    {
        return group_;
    }

    int Node::getChildCount()
    {
        return group_.children_.size();
    }

    std::list<nodeShPtr> Node::getChildren()
    {
        return group_.getChildren();
    }

    void Node::appendChild(nodeShPtr child)
    {
        group_.append(child);
    }

    void Node::prependChild(nodeShPtr child)
    {
        group_.prepend(child);
    }

    nodeShPtr Node::findNode(int id)
    {
        return group_.findNode(id);
    }

    nodeShPtr Node::findNode(const std::string &name)
    {
        return group_.findNode(name);
    }

    affineShPtr Node::calcTransform()
    {
        return affineShPtr();
    }

    void Node::update(double dt)
    {
        // Recure into the children
        for (auto &&child : group_.children_)
        {
            child->update(dt);
        }
    }

    void Node::visit(nodeShPtr node, TransformStack &transformStack,
                     double interpolation, double width, double height)
    {
        // Checking visibility here would cause any children that are visible
        // to not be rendered.
        // TODO Add parent and children flags for individual control.
        if (!node->visible)
            return;

        transformStack.save();

        // TODO Because position and angles are dependent on lerping we perform
        // interpolation first.
        // node.interpolate(interpolation);

        auto aft = node->calcTransform();

        transformStack.applyTransform(*aft);

        node->render(transformStack.current, width, height);

        // Some of the children may still be visible.
        // TODO add extra flag to choose if parent only or all children are visible
        // this will minimize bubbling.
        // Note: if you want the parent AND children to be invisible then you
        // need to bubble visibility to parent and children.

        for (auto &&nod : node->getChildren())
        {
            // if (node is Filter)
            // {
            //     // TODO add filter.visit(stack, interpolation)
            // }
            // else
            // {
            // Recurse down the tree.
            visit(nod, transformStack, interpolation, width, height);
            // }
        }

        transformStack.restore();
    }

} // namespace Core
