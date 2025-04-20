#include <iostream>

#include <gameapp.hpp>

namespace Game
{

    GameApp::~GameApp()
    {
    }

    int GameApp::construct()
    {
        std::cout << "Constructing game" << std::endl;

        // Create BootScene Node (scene)
        // Create SplashScene Node (scene)
        // Create GameScene Node (scene)
        // Create GameLayer Node as child of GameScene
        // Create SquareNode as a child of GameLayer

        return 0;
    }

    void GameApp::render()
    {
        // Render
        // Clear the colorbuffer
        glClearColor(bgClearColor.r, bgClearColor.g, bgClearColor.b, bgClearColor.a);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0); // no need to unbind it every time
    }

} // namespace Game
