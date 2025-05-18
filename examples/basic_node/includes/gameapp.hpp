#pragma once

#include <app.hpp>
#include <node_manager.hpp>
#include "basic_scene_node.hpp"
#include "environment.hpp"

namespace Game
{
    class GameApp final : public App
    {
    private:
        /* data */
        Core::NodeManager nodeMan{};

        std::shared_ptr<BasicScene> basicScene;

        Core::environmentShPtr environment = std::make_shared<Core::Environment>();

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
