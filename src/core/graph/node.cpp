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

} // namespace Core
