#pragma once

#include <vector>

#include <glad/gl.h>

#include "shader.hpp"
#include "basic_shader.hpp"
#include "constants.hpp"
#include "environment.hpp"
#include "matrix4.hpp"
#include "ortho_projection.hpp"

namespace Game
{
    class PrimitiveEboTest
    {
    private:
        /* data */
        GLuint VAO, VBO, EBO;
        Core::BasicShader shader{"mono_vertex.glsl", "mono_fragment.glsl"};
        std::string lastError{};

        Core::environmentShPtr environment;

        Core::ErrorConditions _loadShaders();

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

        // Vertex vars
        const std::string uniModel{"model"}; // \x00
        const std::string uniProjection{"projection"};
        const std::string uniView{"view"};

    public:
        PrimitiveEboTest(/* args */);
        ~PrimitiveEboTest();

        // configure
        Core::ErrorConditions configure(Core::environmentShPtr environment);
        Core::ErrorConditions configureUniforms();
        void bind(const std::vector<float> &vertices,
                  const std::vector<unsigned int> &indices);

        void setColor(const std::array<GLfloat, 4> &color);
        // render
        void render(const Core::Matrix4 &model);

        void use();
        void unUse();

        void glCheck(int markId);

        void dispose();
    };

}