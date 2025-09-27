#pragma once

#include <list>
#include <array>
#include <unordered_map>

#include <glad/gl.h>

#include "shape.hpp"
#include "basic_shader.hpp"
#include "constants.hpp"
#include "base_atlas.hpp"
#include "bitmap_font_base.hpp"
#include "matrix4.hpp"
#include "vector3.hpp"

namespace Core
{
    using bitmapFontBaseUnqPtr = std::unique_ptr<BitmapFontBase>;

    /// @brief This atlas is a single group of vertices and multiple groups
    ///        of indices.
    ///
    /// Each group is a single character.
    ///
    /// You pass it a BitmapFontBase type object. It consumes the font and
    /// prepares it for OpenGL and rendering.
    class BitmapVectorFontAtlas final : public BaseAtlas
    {
    private:
        /// @brief // Bake finalizes the Atlas by "baking" the shapes into the buffers.
        /// @return
        ErrorConditions bake();
        ErrorConditions configureUniforms();

        /// @brief This offset is byte offsets not integer numbers.
        /// It represents a "index pointer" where the next group of indices
        /// starts.
        int indicesByteOffset{0};
        // The atlas has shapes and each shape has vertices. These need to be
        // combined into a single array and later copied into GL Buffer.
        // At the same time each shape needs to be updated
        // to adjust element offsets and counts.
        int indiceBlockOffset{0};

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

        bitmapFontBaseUnqPtr fontBase; // Example: DarkroseBitmapFont

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
        // We DO need to retain the indices byte offsets for rendering during
        // glDrawElements. We use them as references into an OpenGL buffered
        // array.
        // Each entry is a character with an offset into a indices buffer.
        //                          Count --|     |--- Offset in bytes
        std::unordered_map<char, std::pair<int, GLuint>> indicesPairData{};

        GLenum primitiveMode{}; // Example: GL_TRIANGLES
        Core::Matrix4 model{true};
        Core::Vector3 position{0.0, 0.0, 0.0};
        Core::Vector3 scale{1.0, 1.0, 1.0};
        float xIncOffset{0};
        float horzOffset{0.0};

        // Buffers
        GLuint vaoID{};
        GLuint vboID{};
        GLuint eboID{};

        BasicShader shader{"mono_vertex.glsl", "mono_fragment.glsl"};

        std::string lastError{};

        bool dirty{true};

        BitmapVectorFontAtlas(/* args */) { std::cout << "BitmapVectorFontAtlas ctor" << std::endl; };

        ~BitmapVectorFontAtlas() = default;

        void initialize(environmentShPtr environment) override;

        /* data */
        ErrorConditions configure();

        /// @brief Takes ownership of provided Atlas
        /// @param fontBase std::unique_ptr<BitmapFontBase>
        void configureFrom(bitmapFontBaseUnqPtr fontBase);

        void use();
        void unUse();

        void dispose();

        void setColor(const std::array<GLfloat, 4> &color);

        void renderChar(char character, std::list<int> offsets, const Matrix4 &model);
        void renderText(const std::list<char> &characters, const Matrix4 &model);
        void renderText(const std::list<char> &characters);
        void render(const Matrix4 &model);
    };

} // namespace Core
