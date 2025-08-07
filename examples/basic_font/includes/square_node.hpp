#pragma once

#include <node.hpp>
#include <node_manager.hpp>
#include <vector2d.hpp>
#include <rectangle.hpp>
#include <drag_state.hpp>

namespace Game
{
    using nodeWkPtr = std::weak_ptr<Core::Node>;

    class SquareNode final : public Core::Node, public std::enable_shared_from_this<SquareNode>
    {
    private:
        /* data */
        int rectangleAtlasId_{};
        float angle{0};
        float angularVelocityRadiansPerSecond{0.01f * Core::DEGREES_TO_RADIANS};

        Core::Rectangle bounds{};
        Core::DragState dragState{};

    public:
        SquareNode(/* args */) = default;
        explicit SquareNode(std::string name, Core::environmentShPtr environment, nodeWkPtr parent = std::weak_ptr<Node>{})
            : Core::Node(name, parent)
        {
            std::cout << "BasicScene Node Constructor: '" << name << "'" << id << std::endl;
            env = environment;
            // dragState.initialize(shared_from_this(), env);
        };
        ~SquareNode() = default;

        int build(Core::NodeManager &nodeMan) override;

        void receiveSignal(Core::NodeSignal signal) override;
        void update(double dt);
        void render(const Core::Matrix4 &model) override;
        bool handleEvent(const Core::IOEvent &event) override;
    };

} // namespace Game
