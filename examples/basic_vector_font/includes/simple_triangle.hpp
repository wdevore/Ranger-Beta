#pragma once

#include <vector>

#include <glad/gl.h>

#include "global_data.hpp"
#include "matrix4.hpp"
#include "shader.hpp"
#include "ortho_projection.hpp"
#include <shape_generator.hpp>

namespace Game
{
    /// @brief A simple triangle that doesn't use a shader
    class SimpleTriangle
    {
    private:
        /* data */
        Core::ShapeGenerator generator{};

        std::vector<float> vertices{};
        std::vector<unsigned int> indices{};

        std::string name = "SimpleTriangle";

        Core::Shader shader{};
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
        void unUse();
        void dispose();
    };

} // namespace Game
