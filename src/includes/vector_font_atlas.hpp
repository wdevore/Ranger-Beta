#pragma once

#include <list>
#include <array>
#include <unordered_map>

#include <glad/gl.h>

#include "shape.hpp"
#include "basic_shader.hpp"
#include "constants.hpp"
#include "base_atlas.hpp"
#include "matrix4.hpp"
#include "vector3.hpp"
#include "shape_generator.hpp"
#include "ortho_projection.hpp"

namespace Core
{
    /// @brief This atlas is a single group of vertices and multiple groups
    ///        of indices.
    ///
    /// Each group is a single character.
    ///
    /// The atlas consumes basic_vector_font.hpp
    class VectorFontAtlas
    {
    private:
        environmentShPtr environment;

        /// @brief // Bake finalizes the Atlas by "baking" the shapes into the buffers.
        /// @return
        ErrorConditions bake();
        ErrorConditions configureUniforms();

        // We DO need to retain the indices byte offsets for rendering during
        // glDrawElements. We use them as references into an OpenGL buffered
        // array.
        // Each entry is a character with an offset into a indices buffer.
        //                          Count --|     |--- Offset in bytes
        std::unordered_map<char, std::pair<int, GLuint>> indicesPairData{};

        void vboBind(int bufferSize, const std::vector<GLfloat> &vertices);
        // Some may call this an "ibo" instead
        void eboBind(int bufferSize, const std::vector<GLuint> &indices);

        // ------- Uniforms ---------------
        // Fragment vars
        const std::string uniColor{"fragColor"};

        // Vertex vars
        const std::string uniModel{"model"}; // \x00
        const std::string uniProjection{"projection"};
        const std::string uniView{"view"};

        GLint modelLoc{};
        GLint colorLoc{};

        std::string name{};

        ErrorConditions _loadShaders();

    public:
        // -----------------------------------------------------
        // Temporary stores
        // -----------------------------------------------------
        // We only have a single shape to hold temporary data. The data
        // only lasts until the data is transferred to OpenGL.
        // shapeShPtr shape;
        // For the Shaking process
        Shape backingShape{};

        // -----------------------------------------------------
        // Rendering vars
        // -----------------------------------------------------
        GLenum primitiveMode{}; // Example: GL_LINES
        Core::Matrix4 model{true};
        Core::Vector3 position{0.0, 0.0, 0.0};
        Core::Vector3 scale{1.0, 1.0, 1.0};
        float xIncOffset{0};
        float horzOffset{0.0};

        const GLuint UnBindID{0};
        const GLsizei Uniform4vColorCompCount{1};
        /// TODO Add docs on this
        const GLsizei GLUniformMatrixCount{1};
        /// TODO Add docs on this
        const GLboolean GLUniformMatrixTransposed{false};

        // Buffers
        GLuint vaoID{};
        GLuint vboID{};
        GLuint eboID{};

        BasicShader shader{"mono_vertex.glsl", "mono_fragment.glsl"};

        /// @brief This sets the project and positions the Origin.
        OrthoProjection projection{};

        std::string lastError{};

        bool dirty{true};

        VectorFontAtlas(/* args */) { std::cout << "VectorFontAtlas ctor" << std::endl; };

        ~VectorFontAtlas() = default;

        void initialize(environmentShPtr environment);

        /* data */
        ErrorConditions configure(const std::unordered_map<char, std::list<float>> &fontData);

        void use();
        void unUse();

        void dispose();

        void setColor(const std::array<GLfloat, 4> &color);

        void renderChar(char character, const Matrix4 &model);
        void renderText(const std::list<char> &characters, const Matrix4 &model);
        void renderText(const std::list<char> &characters);
        void render(const Matrix4 &model);
    };

} // namespace Core
