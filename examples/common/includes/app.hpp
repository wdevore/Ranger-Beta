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

    protected:
        Color4 bgClearColor{0.2f, 0.3f, 0.3f, 1.0f};

        unsigned int VBO, VAO;
        unsigned int shaderProgram;

        const char *vertexShaderSource = "#version 330 core\n"
                                         "layout (location = 0) in vec3 aPos;\n"
                                         "void main()\n"
                                         "{\n"
                                         "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                         "}\0";
        const char *fragmentShaderSource = "#version 330 core\n"
                                           "out vec4 FragColor;\n"
                                           "void main()\n"
                                           "{\n"
                                           "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                           "}\n\0";

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
        virtual void render() = 0;
    };

} // namespace Game
