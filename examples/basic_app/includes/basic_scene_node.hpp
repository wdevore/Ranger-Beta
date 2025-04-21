#pragma once

#include <memory>

#include <node.hpp>

namespace Game
{
    using nodeWkPtr = std::weak_ptr<Core::Node>;

    class BasicScene : public Core::Node
    {
    private:
        /* data */
    public:
        BasicScene() = default;
        BasicScene(std::string name, nodeWkPtr parent = std::weak_ptr<Node>{})
            : Core::Node(name, parent)
        {
            std::cout << "BasicScene Node Constructor: '" << name << "'" << id << std::endl;
        };
        ~BasicScene() = default;

        void receiveSignal(Core::NodeSignal signal) override;
    };

} // namespace Game
