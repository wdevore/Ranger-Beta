#include <iostream>
#include <memory>

#include <gameapp.hpp>
#include <basic_scene_node.hpp>
#include <basic_shader.hpp>
#include <shape_generator.hpp>
#include <constants.hpp>
#include <environment.hpp>
#include <darkrose_bitmap_font.hpp>
#include <basic_vector_font.hpp>

namespace Game
{
    GameApp::~GameApp()
    {
    }

    /// @brief This is the earliest method called.
    /// mostly for testing and debugging.
    void GameApp::preSetup()
    {
    }

    int GameApp::verifyConfigured()
    {
        std::cout << "GameApp::verifyConfigured" << std::endl;

        if (nodeMan.nodes.size() < 1)
        {
            std::cout << "App is not configured correctly. There must be at least 1 node." << std::endl;
            return -1;
        }

        return 0;
    }

    int GameApp::setup()
    {
        std::cout << "GameApp::setup" << std::endl;

        text = "Basic Font";
        textToList(text);

        // Load any assets, for example vector font.

        nodeMan.initialize();

        return 1;
    }

    int GameApp::construct()
    {
        std::cout << "GameApp::construct game" << std::endl;

        // ----------------------------------------------------------
        // Setup environment
        // Constructs Atlases etc.
        // ----------------------------------------------------------
        auto env = environment;
        env->initialize(width, height);

        // ----------------------------------------------------------
        // Setup Scenes and Nodes
        // ----------------------------------------------------------
        // Create a single scene (aka Node) to hold a square.
        // The Scene will have a background square for color.
        basicScene = std::make_shared<BasicScene>("BasicScene", env);
        basicScene->build(nodeMan);

        nodeMan.push(basicScene);

        // ----------------------------------------------------------
        // Setup Bitmap font
        // ----------------------------------------------------------
        // The font generates the vertices and indices
        auto font = std::make_unique<DarkroseBitmapFont>();
        font->build();
        env->bitmapFontAtlas.configureFrom(std::move(font));
        env->bitmapFontAtlas.scale.set(25.0, 25.0, 1.0);
        env->bitmapFontAtlas.position.set(20.0, 20.0, 0.0);
        env->bitmapFontAtlas.horzOffset = 25.0;
        // ----------------------------------------------------------

        // ----------------------------------------------------------
        // Setup vector font
        // ----------------------------------------------------------
        // The font generates the vertices and indices

        // Use vertices/indices to configure the atlas buffers
        // env->vectorFontAtlas.configure(VectorFont::fontData);

        // env->vectorFontAtlas.scale.set(25.0, 25.0, 1.0);
        // env->vectorFontAtlas.position.set(20.0, 20.0, 0.0);
        // env->vectorFontAtlas.horzOffset = 25.0;
        // ----------------------------------------------------------

        // ---------- Primitive debug tester
        // _eboTest.configure(environment);

        env->postInitialize();

        env->atlas.setColor({1.0, 0.5, 0.0, 0.0});

        return 1;
    }

    void GameApp::render()
    {
        // Render
        nodeMan.visit(0.0, width, height);

        auto env = environment;

        // ------------ Bitmap vector font
        env->bitmapFontAtlas.use();
        env->bitmapFontAtlas.setColor({1.0, 1.0, 0.0, 0.0});
        env->bitmapFontAtlas.renderText(textList);
        env->bitmapFontAtlas.unUse();

        // ------------ Vector font
        Core::Matrix4 model{true};
        model.toIdentity();
        model.translate(125.0, 125.0, 0.0);
        model.scaleBy(25.0, 25.0, 1.0);

        // env->vectorFontAtlas.use();
        // env->vectorFontAtlas.setColor({1.0, 1.0, 1.0, 1.0});
        // env->vectorFontAtlas.renderChar('#', model);
        // env->vectorFontAtlas.unUse();

        // ---------- Primitive debug tester
        // _eboTest.use();
        // _eboTest.setColor({1.0, 1.0, 1.0, 0.0});
        // _eboTest.render(model);
        // _eboTest.unUse();
    }

    int GameApp::update(double dt)
    {
        nodeMan.update(dt);

        return 1; // Successful update
    }

    void GameApp::processIOEvent()
    {
        nodeMan.processEvent(ioEvent);
    }

    int GameApp::deconstruct()
    {
        std::cout << "GameApp::deconstruct" << std::endl;
        environment->dispose();

        return 1; // success
    }

    void GameApp::textToList(const std::string &text)
    {
        for (char c : text)
            textList.push_back(c);
    }

} // namespace Game
