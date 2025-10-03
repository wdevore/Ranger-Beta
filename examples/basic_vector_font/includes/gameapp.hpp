#pragma once

#include <string>
#include <list>

#include <app.hpp>
#include <node_manager.hpp>
#include "basic_scene_node.hpp"
#include "simple_square.hpp"
#include "simple_triangle.hpp"

namespace Game
{
    class GameApp final : public App
    {
    private:
        /* data */
        Core::NodeManager nodeMan{};

        std::shared_ptr<BasicScene> basicScene;

        // std::unique_ptr<Core::BitmapVectorFontAtlas> fontAtlas = std::make_unique<Core::BitmapVectorFontAtlas>();

        std::string text{};
        std::list<char> textList{};

        void textToList(const std::string &text);

        // -------------- DEBUG ----------------
        SimpleSquare _square{};
        SimpleTriangle _triangle{};

    public:
        GameApp() = default;
        GameApp(unsigned int width, unsigned int height) : App(width, height) {};
        ~GameApp();

        void preSetup() override;

        int verifyConfigured() override;

        int deconstruct() override;

        int setup() override;
        int construct() override;
        int update(double dt) override;

        void render() override;
        void processIOEvent() override;
    };

} // namespace Game
