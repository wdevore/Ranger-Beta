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
        /* data */
        ErrorConditions configure();
        ErrorConditions shake();
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
        // We only have a single shape.
        shapeShPtr shape;
        // "GLvoid *" is a pointer to an indice offset
        std::unordered_map<std::string, GLvoid *> shapes{};

        // For the Shaking process
        Shape backingShape{};

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

        /// @brief Main method to call to load and compile.
        /// @return
        ErrorConditions burn();

        /// @brief Takes ownership of provided Atlas
        /// @param fontAtlas
        void configureFrom(bitmapFontBaseUnqPtr fontBase);

        /// @brief Same as above.
        /// @param name
        /// @param vertices
        /// @param indices
        /// @param mode
        /// @return
        int addShape(std::string name,
                     const std::vector<GLfloat> &vertices,
                     std::vector<GLuint> &indices,
                     GLenum mode);

        /// @brief Adds a shape dynamically to the atlas.
        /// @param shape
        /// @return
        int shakeShape(Shape &shape);

        int getIndicesOffset() const;

        void use();
        void unUse();

        void dispose();

        void setColor(const std::array<GLfloat, 4> &color);

        void renderText(std::string text, const Matrix4 &model);
        void render(const Matrix4 &model);
    };

} // namespace Core
