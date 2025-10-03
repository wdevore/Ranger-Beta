#pragma once

#include <vector>

#include <glad/gl.h>

#include "environment.hpp"
#include "matrix4.hpp"

namespace Game
{
    /// @brief A simple triangle that doesn't use a shader
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

        std::string name = "SimpleTriangle";
        Core::environmentShPtr environment;

        Core::BasicShader shader{"mono_vertex.glsl", "mono_fragment.glsl"};
        // Core::BasicShader shader{"basic.vs", "basic.frag"};
        std::string lastError{};

        unsigned int VBO, VAO, EBO;

        GLint modelLoc{};
        GLint colorLoc{};

        /// TODO Add docs on this
        const GLsizei GLUniformMatrixCount{1};
        /// TODO Add docs on this
        const GLboolean GLUniformMatrixTransposed{false};

        const GLsizei Uniform4vColorCompCount{1};

        /// @brief This sets the project and positions the Origin.
        Core::OrthoProjection projection{};

        // ------- Uniforms ---------------
        // Fragment vars
        const std::string uniColor{"fragColor"};

        const std::string uniModel{"model"}; // \x00
        const std::string uniProjection{"projection"};
        const std::string uniView{"view"};

    public:
        SimpleTriangle(/* args */);
        ~SimpleTriangle();

        void configure(Core::environmentShPtr environment);
        void loadCompileLinkShaders();
        void bind();
        Core::ErrorConditions configureUniforms();

        void setColor(const std::array<GLfloat, 4> &color);
        void begin();

        void render(const Core::Matrix4 &model);
        void use();
        void unUse();
    };

} // namespace Game
