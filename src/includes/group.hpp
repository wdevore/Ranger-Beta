#pragma once

#include <list>
#include <memory> // Shared pointers

namespace Core
{
    class Node;
    using nodeShPtr = std::shared_ptr<Node>;

    class Group final
    {
    private:
        /* data */
    public:
        std::list<nodeShPtr> children_{};

        Group(/* args */) = default;
        ~Group() = default;

        std::list<nodeShPtr> getChildren();

        nodeShPtr findNode(int id);
        nodeShPtr findNode(const std::string &name);
        void append(nodeShPtr node);
        void prepend(nodeShPtr node);
        nodeShPtr removeBack();

        bool isEmpty();
        bool isNotEmpty();
    };

} // namespace Core
