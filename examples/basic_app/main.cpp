#include <iostream>

#include <glad/gl.h>
// We don't need to include opengl <gl.h> because glfw does it already.
#include <GLFW/glfw3.h>

// Window dimensions of GL canvas, not the OS window.
const GLuint WIDTH = 800, HEIGHT = 600;

#include <gameapp.hpp>

int main(int, char **)
{
    std::cout << "Hello, App!" << std::endl;

    Game::GameApp app{WIDTH, HEIGHT};

    int initialized = app.initialize();
    if (initialized == GLFW_FALSE)
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    bool windowCreated = app.createWindow("Basic App");
    if (!windowCreated)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        return -1;
    }

    int configured = app.configure();
    if (configured < 0)
    {
        std::cout << "Failed to configure OpenGL" << std::endl;
        return -1;
    }

    app.construct();

    int runState = app.run();

    std::cout << "Done. Goodbye!" << std::endl;
}