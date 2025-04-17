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

    void Node::update(double dt)
    {
        // Recure into the children
        for (auto &&child : group_.children_)
        {
            child->update(dt);
        }
    }

    void Node::visit(TransformStack &transformStack,
                     double interpolation, double width, double height)
    {
        // Checking visibility here would cause any children that are visible
        // to not be rendered.
        // TODO Add parent and children flags for individual control.
        if (!visible)
            return;

        std::cout << "Node::visit save T stack" << std::endl;
        transformStack.save();

        // TODO Because position and angles are dependent on lerping we perform
        // interpolation first.
        // node.interpolate(interpolation);

        auto aft = transform_.calcTransform(dirty);
        std::cout << aft << std::endl;

        transformStack.applyTransform(aft);

        render(transformStack.current, width, height);

        // Some of the children may still be visible.
        // TODO add extra flag to choose if parent only or all children are visible
        // this will minimize bubbling.
        // Note: if you want the parent AND children to be invisible then you
        // need to bubble visibility to parent and children.

        for (auto &&nod : getChildren())
        {
            // if (node is Filter)
            // {
            //     // TODO add filter.visit(stack, interpolation)
            // }
            // else
            // {
            // Recurse down the tree.
            std::cout << "Node::visit recurse into: " << *nod << std::endl;
            nod->visit(transformStack, interpolation, width, height);
            // }
        }

        std::cout << "Node::visit restore T stack : " << *this << std::endl;
        transformStack.restore();
    }

    std::ostream &operator<<(std::ostream &os, NodeSignal s)
    {
        switch (s)
        {
        case NodeSignal::requestNodeLeaveStage:
            os << "requestNodeLeaveStage";
            break;
        case NodeSignal::leaveStageGranted:
            os << "leaveStageGranted";
            break;
        case NodeSignal::nodeMovedToStage:
            os << "nodeMovedToStage";
            break;
        default:
            os << "Unknown NodeSignal";
            break;
        }
        return os;
    }

} // namespace Core
