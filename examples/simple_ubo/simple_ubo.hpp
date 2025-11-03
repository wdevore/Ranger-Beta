#pragma once

#include <vector>
#include <array>

#include <glad/gl.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

#include <shader.hpp>
#include <ortho_projection.hpp>
namespace Game
{
    class SimpleUbo
    {
    private:
        int deviceWidth{0};
        int deviceHeight{0};

        // gameEnvironmentShPtr environment;

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

        GLuint VBO, VAO, EBO;

        // Core::BasicShader shader{"mono_vertex.glsl", "mono_fragment.glsl"};
        Core::Shader shader{"mono_vert_ubo.glsl", "mono_fragment.glsl"};
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

        GLint fetchUniformVar(GLuint program, const std::string &name);

    public:
        /// @brief This sets the project and positions the Origin.
        Core::OrthoProjection projection{};

        SimpleUbo(/* args */);
        ~SimpleUbo();

        GLuint getProgram() { return program; }

        void configure();
        void bind();
        void use();
        void fetchUniforms();

        void setColor(const std::array<GLfloat, 4> &color);
        int begin();
        void render(const Core::Matrix4 &model);

        void dispose();
    };

} // namespace Game
