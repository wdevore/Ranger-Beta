#include <iostream>
#include <memory>

#include <gameapp.hpp>
#include <basic_scene_node.hpp>
#include <basic_shader.hpp>
#include <shape_generator.hpp>
#include <constants.hpp>

namespace Game
{
    GameApp::~GameApp()
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

    // Called by App::configure
    int GameApp::compile()
    {
        std::cout << "GameApp::compile shaders" << std::endl;

        // shader.build();

        return 1; // success
    }

    // Called by App::configure
    int GameApp::build()
    {
        std::cout << "GameApp::build Atlas" << std::endl;

        return 1; // Success
    }

    int GameApp::deconstruct()
    {
        std::cout << "GameApp::deconstruct" << std::endl;
        atlas.dispose();

        return 1; // success
    }

    int GameApp::setup()
    {
        std::cout << "GameApp::setup" << std::endl;

        // Load any assets, for example vector font.

        // Build vector objects
        // vecObj.construct();

        // Initialize any Node objects

        nodeMan.initialize();

        return 1;
    }

    int GameApp::construct()
    {
        std::cout << "GameApp::construct game" << std::endl;

        // Create a single scene Node to hold a square.
        basicScene = std::make_shared<BasicScene>("BasicScene");
        nodeMan.push(basicScene);

        // TODO Hack for now. Add a shape at this point.
        Core::ShapeGenerator generator{};
        generator.generateUnitRectangle(Core::ShapeControls::Centered, Core::ShapeControls::Filled);

        Core::Shape &shape = generator.shape;

        atlas.addShape(shape.name, shape.vertices, shape.indices, shape.primitiveMode);

        atlas.burn();
        atlas.use();

        atlas.setColor({1.0, 0.5, 0.0, 0.0});

        return 1;
    }

    void GameApp::render()
    {
        // std::cout << "rendering" << std::endl;

        // Render
        // nodeMan.visit(0.0, width, height);

        // Clear the colorbuffer
        glClearColor(bgClearColor.r, bgClearColor.g, bgClearColor.b, bgClearColor.a);
        glClear(GL_COLOR_BUFFER_BIT);
        // 1) Check projection and model
        // 2) Switch back to simpler shaders

        Core::Matrix4 model{true};
        atlas.render(0, model);

        // draw our first triangle
        // glUseProgram(shaderProgram);
        // glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

        // glDrawArrays(GL_TRIANGLES, 0, 3);  <---

        // glBindVertexArray(0); // no need to unbind it every time
    }

    int GameApp::update(double dt)
    {

        nodeMan.update(dt);

        return 1; // Successful update
    }

} // namespace Game
