#pragma once

#include <vector>

#include <glad/gl.h>

#include "environment.hpp"
#include "matrix4.hpp"

namespace Game
{
    /// @brief A simple triangle that doesn't use a shader
    class SimpleSquare
    {
    private:
        /* data */
        std::vector<float> vertices{
            0.5f, 0.5f, 0.0f,   // top right
            0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f, // bottom left
            -0.5f, 0.5f, 0.0f   // top left
        };
        std::vector<unsigned int> indices{
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };

        std::string name = "SimpleSquare";
        Core::environmentShPtr environment;

        // Core::BasicShader shader{"mono_vertex.glsl", "mono_fragment.glsl"};
        Core::BasicShader shader{"basic.vs", "basic.frag"};
        std::string lastError{};

        unsigned int VBO, VAO, EBO;

    public:
        SimpleSquare(/* args */);
        ~SimpleSquare();

        void configure(Core::environmentShPtr environment);
        void loadCompileLinkShaders();
        void bind();

        void render(const Core::Matrix4 &model);
    };

} // namespace Game
