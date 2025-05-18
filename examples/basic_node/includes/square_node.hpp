#pragma once

#include <node.hpp>
#include <node_manager.hpp>

namespace Game
{
    using nodeWkPtr = std::weak_ptr<Core::Node>;

    class SquareNode final : public Core::Node, public std::enable_shared_from_this<SquareNode>
    {
    private:
        /* data */
        int rectangleAtlasId_{};
        float angle{0};
        float angularVelocityRadiansPerSecond{0.05f * Core::DEGREES_TO_RADIANS};

    public:
        SquareNode(/* args */) = default;
        explicit SquareNode(std::string name, Core::environmentShPtr environment, nodeWkPtr parent = std::weak_ptr<Node>{})
            : Core::Node(name, parent)
        {
            std::cout << "BasicScene Node Constructor: '" << name << "'" << id << std::endl;
            env = environment;
        };
        ~SquareNode() = default;

        int build(Core::NodeManager &nodeMan) override;

        void receiveSignal(Core::NodeSignal signal) override;
        void update(double dt);
        void render(const Core::Matrix4 &model) override;
    };

} // namespace Game
