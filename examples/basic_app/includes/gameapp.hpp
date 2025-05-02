#pragma once

#include <app.hpp>
#include <node_manager.hpp>
#include <basic_scene_node.hpp>

namespace Game
{
    class GameApp : public App
    {
    private:
        /* data */
        Core::NodeManager nodeMan{};

        std::shared_ptr<BasicScene> basicScene;

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
        GameApp() = default;
        GameApp(unsigned int width, unsigned int height) : App(width, height) {};
        ~GameApp();

        int verifyConfigured() override;

        int compile() override;
        int build() override;
        int deconstruct() override;

        int setup() override;
        int construct() override;
        int update(double dt) override;
        void render() override;
    };

} // namespace Game
