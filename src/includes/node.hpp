#pragma once

#include <string>

#include <group.hpp>

namespace Core
{
    static int NodeID = 0;

    using nodeShPtr = std::shared_ptr<Node>;
    using nodeWkPtr = std::weak_ptr<Node>;

    class Node
    {
    private:
        Group group_;

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
        };

        ~Node() = default;

        void initialize();

        Group &getChildren();
        int getChildCount();

        // Add to back
        void appendChild(nodeShPtr child);
        // Add to front
        void prependChild(nodeShPtr child);
    };

} // namespace Core
