#include <group.hpp>

#include <node.hpp>

namespace Core
{

    nodeShPtr Group::getById(int id)
    {
        for (const auto &ptr : children)
        {
            if (ptr->id == id)
                return ptr;
        }

        return nullptr;
    }

    nodeShPtr Group::getByName(const std::string &name)
    {
        for (const auto &ptr : children)
        {
            if (ptr->name == name)
                return ptr;
        }
        return nullptr;
    }

    void Group::append(nodeShPtr node)
    {
        children.push_back(node);
    }

    void Group::prepend(nodeShPtr node)
    {
        children.push_front(node);
    }
} // namespace Core
