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

    void GameApp::preSetup()
    {
        DarkroseBitmapFont font{};
        font.build();

        const float unitLength{1.0};
        const int numberOfSquaresPerSide{8};
        const int numberOfGaps = numberOfSquaresPerSide - 1;
        const float gapSize{0.025}; // d
        const float sideLenth =
            (unitLength - (numberOfGaps * gapSize)) / numberOfSquaresPerSide; // s

        float my{0.5}; // Initial start value
        float y1 = my; // Start y position
        float y2{0.0}; // End y position

        float y{0.0};

        for (int row = 0; row < numberOfSquaresPerSide + 1; row++)
        {
            // std::cout << "row: " << row << std::endl;
            my -= sideLenth;
            y2 = my;
            y = y1;

            std::cout << "Y vertex from: " << y1 << " to: " << y2 << std::endl;
            // An edge doesn't have a gap
            if (row < numberOfGaps)
                my -= gapSize;
            y1 = my;

            float mx{-0.5}; // Initial start value
            float x1 = mx;  // Start X position
            float x2{0.0};  // End x position

            for (int col = 0; col < numberOfSquaresPerSide; col++)
            {
                // std::cout << "col: " << col << std::endl;
                mx += sideLenth;
                x2 = mx;

                // std::cout << "X vertex from: " << x1 << " to: " << x2 << std::endl;
                // ------- Output ----------------
                std::cout << "Edge: (" << x1 << "," << y << ")-(" << x2 << "," << y << ")" << std::endl;
                // An edge doesn't have a gap
                if (col < numberOfGaps)
                    mx += gapSize;
                x1 = mx;
            }
        }
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

        auto env = environment;
        env->initialize(width, height);

        // Create a single scene (aka Node) to hold a square.
        // The Scene will have a background square for color.
        basicScene = std::make_shared<BasicScene>("BasicScene", env);
        basicScene->build(nodeMan);

        nodeMan.push(basicScene);

        env->postInitialize();

        env->atlas.setColor({1.0, 0.5, 0.0, 0.0});

        return 1;
    }

    void GameApp::render()
    {
        // Render
        nodeMan.visit(0.0, width, height);

        // Core::Matrix4 model{true};
        // model.setScale(100.0, 100.0, 1.0);
        // atlas->setColor({1.0, 0.5, 0.0, 0.0});
        // atlas->render(0, model); // rectangle

        // glBindVertexArray(0); // no need to unbind it every time
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
