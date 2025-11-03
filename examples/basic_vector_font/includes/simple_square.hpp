#pragma once

#include <vector>

#include <glad/gl.h>

#include "matrix4.hpp"
#include "shader.hpp"
#include "ortho_projection.hpp"
#include <shape_generator.hpp>

// Simple Square is a complete Atlas (buffers and shader)
namespace Game
{
    /// @brief A simple triangle that doesn't use a shader
    class SimpleSquare
    {
    private:
        /* data */
        Core::ShapeGenerator generator{};

        std::vector<float> vertices{};
        std::vector<unsigned int> indices{};

        std::string name = "SimpleSquare";

        Core::Shader shader{};
        std::string lastError{};

        unsigned int VBO, VAO, EBO;

        GLint modelLoc{};
        GLint colorLoc{};

        // ------- Uniforms ---------------
        // Fragment vars
        const std::string uniColor{"fragColor"};
        const std::string uniModel{"model"}; // \x00
        const std::string uniProjection{"projection"};
        const std::string uniView{"view"};

        const GLsizei Uniform4vColorCompCount{1};

        /// TODO Add docs on this
        const GLsizei GLUniformMatrixCount{1};
        /// TODO Add docs on this
        const GLboolean GLUniformMatrixTransposed{false};

        /// @brief This sets the project and positions the Origin.
        Core::OrthoProjection projection{};

    public:
        SimpleSquare(/* args */);
        ~SimpleSquare();

        void configure();
        void loadCompileLinkShaders();
        void bind();
        Core::ErrorConditions configureUniforms();

        GLuint getProgram() { return shader.program(); }
        void setProjectionView();
        GLint fetchUniformVar(GLuint program, const std::string &programName);

        void begin();
        void use();
        void setColor(const std::array<GLfloat, 4> &color);
        void render(const Core::Matrix4 &model);
        void dispose();
    };

} // namespace Game
