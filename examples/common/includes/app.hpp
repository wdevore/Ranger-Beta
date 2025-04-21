#pragma once

#include <glad/gl.h>
// We don't need to include opengl <gl.h> because glfw does it already.
#include <GLFW/glfw3.h>

#include <string>

#include <color4.hpp>

namespace Game
{
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

        App() = default;
        App(unsigned int width, unsigned int height) : width(width), height(height) {};
        ~App();

        int initialize();
        bool createWindow(const std::string &title);
        int configure();

        /// @brief Start game loop
        /// @return
        int run();

        /// @brief Called after App has initialized
        /// @return
        virtual int setup() = 0;
        /// @brief Called when it is time to contruct your game nodes
        /// @return
        virtual int construct() = 0;
        /// @brief Called every time an update is required
        /// @param dt
        /// @return
        virtual int update(double dt) = 0;
        /// @brief Called every frame to draw your game
        virtual void render() = 0;
    };

} // namespace Game
