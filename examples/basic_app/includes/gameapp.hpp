#pragma once

#include <app.hpp>
#include <node_manager.hpp>
#include <static_mono_atlas.hpp>
#include "basic_scene_node.hpp"
// #include "vector_object.hpp"
// #include "basic_shader.hpp"

namespace Game
{
    class GameApp : public App
    {
    private:
        /* data */
        Core::NodeManager nodeMan{};

        std::shared_ptr<BasicScene> basicScene;

        Core::StaticMonoAtlas atlas{};

    public:
        GameApp() = default;
        GameApp(unsigned int width, unsigned int height) : App(width, height) {};
        ~GameApp();

        int verifyConfigured() override;

        int compile() override;
        int build() override;
        int deconstruct() override;

        int setup() override;
        int construct() override;
        int update(double dt) override;
        void render() override;
    };

} // namespace Game
