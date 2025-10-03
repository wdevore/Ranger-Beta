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

        text = "Basic Vector Font";
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
        // ----------------------------------------------------------
        auto env = environment;
        env->initialize(width, height);

        // ----------------------------------------------------------
        // Setup Scenes and Nodes
        // ----------------------------------------------------------
        // Create a single scene (aka Node) to comply with framework.
        // The scene simply sets the background color.
        basicScene = std::make_shared<BasicScene>("BasicScene", env);
        basicScene->build(nodeMan);

        nodeMan.push(basicScene);

        // ---------- Primitive debug tester
        _square.configure(environment);

        env->postInitialize();

        env->atlas.setColor({1.0, 0.5, 0.0, 0.0});

        return 1;
    }

    void GameApp::render()
    {
        // Render
        nodeMan.visit(0.0, width, height);

        auto env = environment;

        // ------------ Vector font
        Core::Matrix4 model{true};
        model.toIdentity();
        model.translate(125.0, 125.0, 0.0);
        model.scaleBy(25.0, 25.0, 1.0);

        Color4 bgClearColor{0.2f, 0.2f, 0.2f, 1.0f};
        glClearColor(bgClearColor.r, bgClearColor.g, bgClearColor.b, bgClearColor.a);
        Core::checkGLError("GameApp::render:glClearColor");

        glClear(GL_COLOR_BUFFER_BIT);
        Core::checkGLError("GameApp::render:glClear");

        // ---------- Primitive debug tester
        // _triangle.begin();
        // _triangle.use();
        // _triangle.setColor({1.0, 1.0, 1.0, 0.0});
        // _triangle.render(model);

        _square.render(model);
        // _square.unUse();
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
