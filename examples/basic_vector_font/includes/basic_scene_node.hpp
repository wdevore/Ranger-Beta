#pragma once

#include <memory>

#include <node.hpp>
#include <matrix4.hpp>
#include <node_manager.hpp>
#include <color4.hpp>
#include <io_event.hpp>
#include <zoom_node.hpp>

namespace Game
{
    using nodeWkPtr = std::weak_ptr<Core::Node>;

    class BasicScene final : public Core::Node, public std::enable_shared_from_this<BasicScene>
    {
    private:
        /* data */
        Core::Color4 bgClearColor{0.2f, 0.2f, 0.2f, 1.0f};

    public:
        BasicScene() = default;
        explicit BasicScene(std::string name, nodeWkPtr parent = std::weak_ptr<Node>{})
            : Core::Node(name, parent)
        {
            std::cout << "BasicScene Node Constructor: '" << name << "'" << id << std::endl;
        };
        ~BasicScene() = default;

        int build(Core::NodeManager &nodeMan) override;
        void update(double dt) override;

        // void update(double dt) override;
        void render(const Core::Matrix4 &model) override;

        void receiveSignal(Core::NodeSignal signal) override;

        bool handleEvent(const Core::IOEvent &event) override;
    };

} // namespace Game
