#pragma once

// #include <map>
#include <list>
#include <string>

#include "transform_stack.hpp"
#include "node.hpp"
#include "constants.hpp"

namespace Core
{
    using nodeShPtr = std::shared_ptr<Node>;

    /// @brief
    /// The NodeManager controls Node traversal.
    ///
    /// Holds and manages the Nodes that represent the visual aspect of the Scene.
    class NodeManager
    {
    private:
        /* data */
    public:
        // Clear or render background. Some nodes take the entire render area
        // so clearing or render a background is wasteful.
        bool clearBackground{false};

        // Matrix stack for recursive decent.
        TransformStack transformStack{};

        // The map tracks if nodes are present in the nodeStack
        // std::map<std::string, nodeShPtr> nodeMap{};

        // The stack is used for Nodes that push other nodes onto the stack.
        // The Top is always render, so once a Node is pushed it is rendered.
        std::list<nodeShPtr> nodes{};
        bool popTheTop{false};

        //      Underlay           stack Top              Overlay
        nodeShPtr underlay = nullptr;
        nodeShPtr overlay = nullptr;

        NodeManager(/* args */) = default;
        ~NodeManager() = default;

        void initialize();
        void update(double msPerUpdate);
        void visit(double interpolation, double width, double height);

        void pushBack(nodeShPtr node);
        void pushFront(nodeShPtr node);
        void removeNode(const std::string &name);

        bool isNodeOnStage(nodeShPtr node);

        ErrorConditions sendSignal(nodeShPtr node, NodeSignal signal);
    };

} // namespace Core
