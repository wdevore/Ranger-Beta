#pragma once

#include <iostream>
#include <string>

#include <group.hpp>
#include <transform.hpp>

namespace Core
{
    static int NodeID{0};

    using nodeShPtr = std::shared_ptr<Node>;
    using nodeWkPtr = std::weak_ptr<Node>;

    class Node
    {
    private:
        Group group_;
        Transform transform_;

    public:
        int id{0};
        std::string name{};

        bool visible{true};
        bool dirty{true};

        nodeWkPtr parent;

        // This explicitly constructs a default-initialized std::weak_ptr,
        // which will be empty (i.e., not associated with any managed object,
        // similar to a null raw pointer). This is done because you can't
        // assign a default nullptr value to a weak_ptr.
        // If a shared_ptr is passed this it is converted to a weak_ptr via
        // a copy-constructor.
        Node(std::string name, nodeWkPtr parent = std::weak_ptr<Node>{})
            : name(name), parent(parent)
        {
            id = NodeID++;
            std::cout << "Node Constructor: '" << name << "'" << id << std::endl;
        };

        ~Node() = default;

        Group &getGroup();
        int getChildCount();
        std::list<nodeShPtr> getChildren();

        // Add to back
        void appendChild(nodeShPtr child);
        // Add to front
        void prependChild(nodeShPtr child);
        nodeShPtr findNode(int id);
        nodeShPtr findNode(const std::string &name);

        // This is equivalent to toString()
        friend std::ostream &operator<<(std::ostream &os, const Node &obj)
        {
            os << ":'" << obj.name << "' (" << obj.id << "):";
            return os;
        }
    };

} // namespace Core
