#pragma once

#include <app.hpp>
#include <node_manager.hpp>
#include <basic_scene_node.hpp>

namespace Game
{
    class GameApp : public App
    {
    private:
        /* data */
        Core::NodeManager nodeMan{};

        std::shared_ptr<BasicScene> basicScene;

    public:
        GameApp() = default;
        GameApp(unsigned int width, unsigned int height) : App(width, height) {};
        ~GameApp();

        int setup() override;
        int construct() override;
        int update(double dt) override;
        void render() override;
    };

} // namespace Game
