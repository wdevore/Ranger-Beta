#pragma once

// #include <map>
#include <list>
#include <string>

#include "transform_stack.hpp"
#include "node.hpp"
#include "constants.hpp"
#include "io_event.hpp"

namespace Core
{
    using nodeShPtr = std::shared_ptr<Node>;

    /// @brief
    /// The NodeManager controls Node traversal.
    ///
    /// Holds and manages the Nodes that represent the visual aspect of the Scene.
    class NodeManager final
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

        // The Top is always rendered, so once a Node is pushed it is rendered.
        std::list<nodeShPtr> nodes{};
        bool popTheTop{false};

        //      Underlay           stack Top              Overlay
        nodeShPtr underlay = nullptr;
        nodeShPtr overlay = nullptr;

        // Nodes that want an update call must register themselves as a target.
        std::list<nodeShPtr> timingTargets{};
        std::list<nodeShPtr> eventTargets{};

        NodeManager(/* args */) = default;
        ~NodeManager() = default;

        void initialize();
        /// @brief
        /// @param dt is in milliseconds
        void update(double dt);
        void visit(double interpolation, double width, double height);

        void push(nodeShPtr node);
        void pushBack(nodeShPtr node);
        void pushFront(nodeShPtr node);
        void pop();
        void removeNode(const std::string &name);

        bool isNodeOnStage(nodeShPtr node);

        void registerTarget(nodeShPtr node);
        void unRegisterTarget(nodeShPtr node);

        void processEvent(const IOEvent &event);
        void registerForEvent(nodeShPtr node);
        void unRegisterForEvent(nodeShPtr node);

        ErrorConditions sendSignal(nodeShPtr node, NodeSignal signal);
    };

} // namespace Core
