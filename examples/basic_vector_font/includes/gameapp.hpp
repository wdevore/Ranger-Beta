#pragma once

#include <string>
#include <list>

#include <app.hpp>

#include <node_manager.hpp>
#include "basic_scene_node.hpp"
#include "simple_square.hpp"
#include "simple_triangle.hpp"

#include "vector_vector_font_atlas.hpp"
// #include "bitmap_vector_font_atlas.hpp"
#include "particle_font_atlas.hpp"
#include "shapes_atlas.hpp"
#include "ubo_buffer.hpp"
#include "shapes_atlas.hpp"

namespace Game
{
    class GameApp final : public App
    {
    private:
        /* data */
        Core::NodeManager nodeMan{};

        std::shared_ptr<BasicScene> basicScene;

        UboBuffer uboBuffer{};

        Core::ShapesAtlas shapesAtlas{};
        Core::shapeShPtr _squareShape;
        Core::shapeShPtr _triangleShape;

        // Core::BitmapVectorFontAtlas bitmapFontAtlas{};
        Core::VectorVectorFontAtlas vectorFontAtlas{};
        Core::ParticleFontAtlas particleFontAtlas{};

        // The "camera" is actually a Core::Matrix4 viewSpace in GlobalData

        std::string text{};
        std::list<char> textList{};

        void textToList(const std::string &text);

        // -------------- DEBUG ----------------
        // SimpleSquare _square{};
        SimpleTriangle _triangle{};

    public:
        GameApp() = default;
        GameApp(const unsigned int width, const unsigned int height) : App(width, height)
        {
            Core::deviceWidth = width;
            Core::deviceHeight = height;
        };
        ~GameApp();

        void setProjection();

        void preSetup() override;
        int setup() override;
        int construct() override;
        int verifyConfigured() override;
        int deconstruct() override;

        int update(double dt) override;
        void render() override;

        void processIOEvent() override;
    };

} // namespace Game
