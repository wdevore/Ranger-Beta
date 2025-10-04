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
        gameEnvironmentShPtr environment;

        /* data */
        // OpenGL's +Y axis is downward
        //            -y
        //             |
        //             |
        //             |
        // -x ------------------- +x
        //             |
        //             |
        //             |
        //            +y
        std::vector<float> vertices{
            -0.5f, 0.5f, 0.0f, // left
            0.5f, 0.5f, 0.0f,  // right
            0.0f, -0.5f, 0.0f  // top
        };
        std::vector<unsigned int> indices{
            0, 1, 2 //
        };

        unsigned int VBO, VAO, EBO;
        // Core::BasicShader shader{"trivial_vertex.glsl", "trivial_fragment.glsl"};
        Core::BasicShader shader{"mono_vertex.glsl", "mono_fragment.glsl"};
        GLuint program{0};

        // Fragment vars
        const std::string uniColor{"fragColor"};

        // Vertex vars
        const std::string uniModel{"model"}; // \x00
        const std::string uniProjection{"projection"};
        const std::string uniView{"view"};

        // Uniform Locations
        GLint colorLoc{0};
        GLint modelLoc{0};
        GLint viewLoc{0};
        GLint projectLoc{0};

        /// TODO Add docs on this
        const GLsizei GLUniformMatrixCount{1};
        /// TODO Add docs on this
        const GLboolean GLUniformMatrixTransposed{false};

        /// @brief This sets the project and positions the Origin.
        Core::OrthoProjection projection{};

        GLint fetchUniformVar(GLuint program, const std::string &name);

    public:
        SimpleTriangle(/* args */);
        ~SimpleTriangle();

        GLuint getProgram() { return program; }

        void configure(gameEnvironmentShPtr environment);
        void bind();
        void use();
        void fetchUniforms();

        void setColor(const std::array<GLfloat, 4> &color);
        int begin();
        void render(const Core::Matrix4 &model);

        void dispose();
    };

} // namespace Game
