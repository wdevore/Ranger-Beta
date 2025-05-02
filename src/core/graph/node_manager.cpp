// #include <algorithm> // for std::find_if
#include <iostream>

#include <node_manager.hpp>
#include <matrix4.hpp>

namespace Core
{
    void NodeManager::initialize()
    {
        transformStack.initialize(Matrix4{true});
    }

    void NodeManager::update(double msPerUpdate)
    {
        if (underlay != nullptr)
            underlay->update(msPerUpdate);

        // Iterate the node stack
        for (auto &&node : nodes)
        {
            node->update(msPerUpdate);
        }

        // We can't pop a node while in the "for" above, so we capture for popping.
        if (popTheTop)
        {
            pop();
            popTheTop = false; // Make sure we don't pop again.
        }

        if (overlay != nullptr)
            overlay->update(msPerUpdate);
    }

    void NodeManager::visit(double interpolation, double width, double height)
    {
        if (underlay != nullptr)
        {
            std::cout << "NM: Visitng underlay" << std::endl;
            underlay->visit(transformStack, interpolation, width, height);
        }

        // Visit node on Stage
        // std::cout << "NM: Visitng top : " << *nodes.front() << std::endl;
        nodes.front()->visit(transformStack, interpolation, width, height);

        if (overlay != nullptr)
        {
            std::cout << "NM: Visitng overlay" << std::endl;
            overlay->visit(transformStack, interpolation, width, height);
        }
    }

    void NodeManager::pushBack(nodeShPtr node)
    {
        nodes.push_back(node);
    }

    /// @brief This exactly like calling pushFront. This is the typical
    /// call rather than explicitly calling pushBack or pushFront.
    /// This also means you need to make sure you push Nodes in reverse
    /// order, for example, you would push a Splash node first then a Boot node.
    /// @param node
    void NodeManager::push(nodeShPtr node)
    {
        pushFront(node);
    }

    void NodeManager::pushFront(nodeShPtr node)
    {
        nodes.push_front(node);
    }

    void NodeManager::pop()
    {
        nodes.pop_front();
    }

    void NodeManager::removeNode(const std::string &name)
    {
        // std::list<nodeShPtr>::iterator it = std::find(nodes.begin(), nodes.end(), name);

        // auto it = std::find_if(nodes.begin(), nodes.end(),
        //                        [name](nodeShPtr n)
        //                        { return n->name == name; });

        nodes.remove_if([name](nodeShPtr n)
                        { return n->name == name; });
    }

    bool NodeManager::isNodeOnStage(nodeShPtr node)
    {
        return node->id == nodes.front()->id;
    }

    ErrorConditions NodeManager::sendSignal(nodeShPtr node, NodeSignal signal)
    {
        // std::cout << "Node Manager::sendSignal: " << signal << std::endl;

        switch (signal)
        {
        case NodeSignal::requestNodeLeaveStage:
            // Node requested to leave stage.
            // Check if on stage first.
            if (isNodeOnStage(node))
            {
                popTheTop = true;

                // Send signal to currently active Node
                node->receiveSignal(NodeSignal::leaveStageGranted);

                if (nodes.size() > 1)
                {
                    // Send signal to the next Node to activate, which is top+1
                    auto it = nodes.cbegin();
                    nodeShPtr next = *(++it);
                    std::cout << "Node Manager::sendSignal: sending sig to Next : " << *next << std::endl;
                    next->receiveSignal(NodeSignal::nodeMovedToStage);
                }
            }
            else
            {
                return ErrorConditions::AttemptingToRemoveNonTopNode;
            }
            break;

        default:
            return ErrorConditions::UnknownSignal;
            break;
        }

        // std::cout << "Node Manager::sendSignal: done : " << *node << std::endl;

        return ErrorConditions::None;
    }

} // namespace Core
