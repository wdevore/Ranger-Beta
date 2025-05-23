#include <iostream>

#include <glad/gl.h>
// We don't need to include opengl <gl.h> because glfw does it already.
#include <GLFW/glfw3.h>

// Window dimensions of GL canvas, not the OS window.
const GLuint DEVICE_WIDTH = 800, DEVICE_HEIGHT = 600;

#include <gameapp.hpp>

int main(int, char **)
{
    std::cout << "Hello, triangle!" << std::endl;

    Game::GameApp app{DEVICE_WIDTH, DEVICE_HEIGHT};

    int initialized = app.initialize();
    if (initialized == GLFW_FALSE)
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    bool windowCreated = app.createWindow("Basic Triangle");
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