#include <iostream>
#include <memory>

#include <gameapp.hpp>
#include <basic_scene_node.hpp>
#include <basic_shader.hpp>
#include <shape_generator.hpp>
#include <constants.hpp>
#include <environment.hpp>
#include <darkrose_bitmap_font.hpp>

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

        // atlas = std::make_shared<Core::StaticMonoAtlas>();

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
        // Setup Bitmap vector font
        // ----------------------------------------------------------
        // The font generates the vertices and indices
        // auto font = std::make_unique<DarkroseBitmapFont>();
        // font->build();

        // // Use vertices/indices to configure the atlas buffers
        // env->fontAtlas.configureFrom(std::move(font));

        // ----------------------------------------------------------
        // Setup Scenes and Nodes
        // ----------------------------------------------------------
        // Create a single scene (aka Node) to hold a square.
        // The Scene will have a background square for color.
        basicScene = std::make_shared<BasicScene>("BasicScene", env);
        basicScene->build(nodeMan);

        nodeMan.push(basicScene);

        Core::ShapeGenerator generator{};
        generator.generateFontChar(Core::ShapeControls::Filled);
        Core::Shape &shape = generator.shape;

        env->atlas.addShape(shape);

        env->postInitialize();

        env->atlas.setColor({1.0, 0.5, 0.0, 0.0});

        return 1;
    }

    void GameApp::render()
    {
        // Render
        nodeMan.visit(0.0, width, height);

        auto env = environment;

        // env->fontAtlas.use();
        // Core::Matrix4 model{true};
        // model.translate(250.0, 250.0, 0.0);
        // model.setScale(100.0, 100.0, 1.0);
        // env->fontAtlas.setColor({1.0, 0.5, 0.0, 0.0});
        // env->fontAtlas.renderText({}, model);
        // env->fontAtlas.unUse();

        env->atlas.use();
        Core::Matrix4 modelR{true};
        modelR.translate(50.0, 50.0, 0.0);
        modelR.scaleBy(50.0, 50.0, 1.0);
        env->atlas.setColor({1.0, 1.0, 0.0, 0.0});
        env->atlas.render(1, modelR); // Char font
        env->atlas.unUse();
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

} // namespace Game
