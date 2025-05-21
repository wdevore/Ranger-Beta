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

    void NodeManager::update(double dt)
    {
        for (auto &&target : timingTargets)
        {
            target->update(dt);
        }

        // if (underlay != nullptr)
        //     underlay->update(msPerUpdate);

        // // Iterate the node stack
        // for (auto &&node : nodes)
        // {
        //     node->update(msPerUpdate);
        // }

        if (popTheTop)
        {
            // First unregister the Top Node if it is registered as a timing target.
            nodeShPtr front = nodes.front();
            std::cout << "Unregistering potential timing target: " << front << std::endl;

            unRegisterTarget(front);

            pop();
            popTheTop = false; // Make sure we don't pop again.
        }

        // if (overlay != nullptr)
        //     overlay->update(msPerUpdate);
    }

    // Visit performs the render pass
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

    void NodeManager::registerTarget(nodeShPtr node)
    {
        timingTargets.push_back(node);
    }

    void NodeManager::unRegisterTarget(nodeShPtr node)
    {
        timingTargets.remove_if([node](nodeShPtr n)
                                { return n->name == node->name; });
    }

    void NodeManager::processEvent(const IOEvent &event)
    {
        for (auto &&target : eventTargets)
        {
            target->handleEvent(event);
        }
    }

    void NodeManager::registerForEvent(nodeShPtr node)
    {
        eventTargets.push_back(node);
    }

    void NodeManager::unRegisterForEvent(nodeShPtr node)
    {
        eventTargets.remove_if([node](nodeShPtr n)
                               { return n->name == node->name; });
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
