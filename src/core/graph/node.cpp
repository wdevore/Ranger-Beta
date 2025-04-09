#include <iostream>

#include "node.hpp"

namespace Core
{
    void Node::initialize()
    {
        std::cout << "Node initialize" << std::endl;
    }

    Group &Node::getChildren()
    {
        return group_;
    }

    int Node::getChildCount()
    {
        return group_.children.size();
    }

    void Node::appendChild(nodeShPtr child)
    {
        group_.append(child);
    }

    void Node::prependChild(nodeShPtr child)
    {
        group_.prepend(child);
    }

} // namespace Core
