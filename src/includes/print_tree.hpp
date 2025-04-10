#pragma once

#include <memory>
#include <list>
#include <string>

#include "node.hpp"

namespace Core
{
    using nodeShPtr = std::shared_ptr<Node>;
    const std::string treeIndent = "   ";

    static void _printBranch(int level, const nodeShPtr &node)
    {
        // If a node's name begins with "::" then don't print it.
        // This is handy for particle systems or parent nodes with
        // lots of cloned children.
        if (node->name.substr(0, 2) == "::")
            return;

        for (int i = 0; i < level; i++)
        {
            std::cout << treeIndent;
        }

        std::cout << node->name << std::endl;
    }

    static void _printSubTree(
        const std::list<nodeShPtr> &nodes,
        const std::list<nodeShPtr> &children,
        int level)
    {
        for (auto &&child : children)
        {
            auto subChildren = child->getGroup().getChildren();
            _printBranch(level, child);

            if (!subChildren.empty())
            {
                _printSubTree(nodes, subChildren, level);
            }
        }
    }

    static void printTree(const std::list<nodeShPtr> &nodes, const nodeShPtr &node)
    {
        std::cout << "------------- Tree -------------------\n";
        if (node != nullptr)
        {
            _printBranch(0, node);

            if (node->getChildCount() > 0)
            {
                auto subChildren = node->getGroup().getChildren();
                _printSubTree(nodes, subChildren, 1);
            }
        }

        std::cout << "------------- Stack -------------------\n";
        for (auto &&node : nodes)
        {
            std::cout << *node << std::endl;
        }

        std::cout << "============ Print Done ========" << std::endl;
    }
} // namespace Core
