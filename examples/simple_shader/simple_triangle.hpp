#pragma once

#include <vector>
#include <array>

#include <glad/gl.h>

#include "game_environment.hpp"
#include <basic_shader.hpp>

namespace Game
{
    class SimpleTriangle
    {
    private:
        /* data */
        std::vector<float> vertices{
            -0.5f, -0.5f, 0.0f, // left
            0.5f, -0.5f, 0.0f,  // right
            0.0f, 0.5f, 0.0f    // top
        };
        std::vector<unsigned int> indices{
            0, 1, 2 //
        };

        unsigned int VBO, VAO, EBO;
        Core::BasicShader shader{"trivial_vertex.glsl", "trivial_fragment.glsl"};
        GLuint program{0};

        GLsizei vertexColorLocation{0};

    public:
        SimpleTriangle(/* args */);
        ~SimpleTriangle();

        GLuint getProgram() { return program; }

        void configure(gameEnvironmentShPtr environment);
        void bind();
        void use();

        void setColor(const std::array<GLfloat, 4> &color);
        void render();

        void dispose();
    };

} // namespace Game
