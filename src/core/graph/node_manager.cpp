// #include <algorithm> // for std::find_if
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
            nodes.pop_front();
            popTheTop = false; // Make sure we don't pop again.
        }

        if (overlay != nullptr)
            overlay->update(msPerUpdate);
    }

    void NodeManager::visit(double interpolation, double width, double height)
    {
        if (underlay != nullptr)
            Node::visit(underlay, transformStack, interpolation, width, height);

        // Visit node on Stage
        Node::visit(nodes.front(), transformStack, interpolation, width, height);

        if (overlay != nullptr)
            Node::visit(overlay, transformStack, interpolation, width, height);
    }

    void NodeManager::pushBack(nodeShPtr node)
    {
        nodes.push_back(node);
    }

    void NodeManager::pushFront(nodeShPtr node)
    {
        nodes.push_front(node);
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

        return ErrorConditions::None;
    }

} // namespace Core
