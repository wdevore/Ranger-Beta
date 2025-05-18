#pragma once

#include <app.hpp>
#include <node_manager.hpp>
#include "basic_scene_node.hpp"
#include <game_environment.hpp>

namespace Game
{
    class GameApp final : public App
    {
    private:
        /* data */
        Core::NodeManager nodeMan{};

        std::shared_ptr<BasicScene> basicScene;

        gameEnvironmentShPtr environment = std::make_shared<GameEnvironment>();

    public:
        GameApp() = default;
        GameApp(unsigned int width, unsigned int height) : App(width, height) {};
        ~GameApp();

        int verifyConfigured() override;

        int deconstruct() override;

        int setup() override;
        int construct() override;
        int update(double dt) override;
        void render() override;
    };

} // namespace Game
