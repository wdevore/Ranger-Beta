#pragma once

#include <list>
#include <memory>

namespace Core
{
    class Node;

    class Group
    {
    private:
        /* data */
    public:
        std::list<std::shared_ptr<Node>> children{};

        Group(/* args */) = default;
        ~Group() = default;

        std::shared_ptr<Node> getById(int id);
        std::shared_ptr<Node> getByName(const std::string &name);
        void append(std::shared_ptr<Node> node);
        void prepend(std::shared_ptr<Node> node);
    };

} // namespace Core
