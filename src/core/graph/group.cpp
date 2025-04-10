#include <group.hpp>

#include <node.hpp>

namespace Core
{

    nodeShPtr Group::findNode(int id)
    {
        for (const auto &ptr : children_)
        {
            if (ptr->id == id)
                return ptr;
        }

        return nullptr;
    }

    std::list<nodeShPtr> Group::getChildren()
    {
        return children_;
    }

    nodeShPtr Group::findNode(const std::string &name)
    {
        for (const auto &ptr : children_)
        {
            if (ptr->name == name)
                return ptr;
        }
        return nullptr;
    }

    void Group::append(nodeShPtr node)
    {
        children_.push_back(node);
    }

    void Group::prepend(nodeShPtr node)
    {
        children_.push_front(node);
    }

    nodeShPtr Group::removeBack()
    {
        nodeShPtr n = children_.back();
        children_.pop_back();
        return n;
    }

    bool Group::isEmpty()
    {
        return children_.empty();
    }

    bool Group::isNotEmpty()
    {
        return !children_.empty();
    }
} // namespace Core
