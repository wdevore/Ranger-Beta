#pragma once

#include <glad/gl.h>
// We don't need to include opengl <gl.h> because glfw does it already.
#include <GLFW/glfw3.h>

#include <string>

namespace Game
{
    struct Color4
    {
        float r{};
        float g{};
        float b{};
        float a{};

        Color4(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
    };

    class App
    {
    private:
        /* data */
        GLFWwindow *window_;
        Color4 bgClearColor{0.2f, 0.3f, 0.3f, 1.0f};

    public:
        unsigned int width{};
        unsigned int height{};

        App(unsigned int width, unsigned int height) : width(width), height(height) {};
        ~App();

        int initialize();
        bool createWindow(const std::string &title);
        int configure();

        int run();

        virtual int construct() = 0;
    };

} // namespace Game
