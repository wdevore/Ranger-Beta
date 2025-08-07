#include <iostream>

#include <glad/gl.h>
// We don't need to include opengl <gl.h> because glfw does it already.
#include <GLFW/glfw3.h>

// Window dimensions of GL canvas, not the OS window.
const GLuint DEVICE_WIDTH = 800, DEVICE_HEIGHT = 600;
const std::string title{"Basic Font"};

#include <gameapp.hpp>

int main(int, char **)
{
    std::cout << "Game launching" << std::endl;

    Game::GameApp app{DEVICE_WIDTH, DEVICE_HEIGHT};

    int initialized = app.initialize();
    if (initialized == GLFW_FALSE)
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    bool windowCreated = app.createWindow(title);
    if (!windowCreated)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        return -1;
    }

    // Main configuration.
    int configured = app.configure();
    if (configured < 0)
    {
        std::cout << "Failed to configure OpenGL" << std::endl;
        return -1;
    }

    app.construct();

    int runStatus = app.run();

    if (runStatus < 0)
    {
        std::cout << "App failed to launch" << std::endl;
    }

    std::cout << "Done. Goodbye!" << std::endl;
}