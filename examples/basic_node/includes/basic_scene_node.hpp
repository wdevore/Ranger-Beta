#pragma once

#include <memory>

#include "node.hpp"
#include "environment.hpp"
#include "matrix4.hpp"
#include "node_manager.hpp"

namespace Game
{
    using nodeWkPtr = std::weak_ptr<Core::Node>;

    class BasicScene final : public Core::Node, public std::enable_shared_from_this<BasicScene>
    {
    private:
        /* data */
        int rectangleAtlasId_{};
        Core::Matrix4 model{};

    public:
        BasicScene() = default;
        explicit BasicScene(std::string name, Core::environmentShPtr environment, nodeWkPtr parent = std::weak_ptr<Node>{})
            : Core::Node(name, parent)
        {
            std::cout << "BasicScene Node Constructor: '" << name << "'" << id << std::endl;
            env = environment;
        };
        ~BasicScene() = default;

        int build(Core::NodeManager &nodeMan) override;

        void render(const Core::Matrix4 &model) override;

        void receiveSignal(Core::NodeSignal signal) override;
    };

} // namespace Game
