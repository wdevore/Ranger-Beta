#include <iostream>
#include <memory>

#include <gameapp.hpp>
#include <basic_scene_node.hpp>

namespace Game
{
    GameApp::~GameApp()
    {
    }

    int GameApp::setup()
    {
        // Load any assets, for example vector font.

        // Initialize any objects
        nodeMan.initialize();

        return 1;
    }

    int GameApp::construct()
    {
        std::cout << "Constructing game" << std::endl;

        // Create a single scene Node to hold a square.
        basicScene = std::make_shared<BasicScene>("BasicScene");

        return 1;
    }

    void GameApp::render()
    {
        // Render
        nodeMan.visit(0.0, width, height);

        // Clear the colorbuffer
        glClearColor(bgClearColor.r, bgClearColor.g, bgClearColor.b, bgClearColor.a);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0); // no need to unbind it every time
    }

    int GameApp::update(double dt)
    {

        nodeMan.update(dt);

        return 1; // Successful update
    }

} // namespace Game
