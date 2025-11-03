#pragma once

#include <list>
#include <utility> // std::pair
#include <unordered_map>

#include "atlas.hpp"
#include "shape.hpp"
#include "matrix4.hpp"
#include "vector3.hpp"

namespace Core
{
    class VectorVectorFontAtlas final : public Atlas
    {
    private:
        GLint colorLoc{};
        const std::string uniColor{"fragColor"};
        // Indicates only 1 color component
        const GLsizei Uniform4vColorCompCount{1};

        ErrorConditions initialAndBuildShader_();

        GLenum primitiveMode{}; // Example: GL_TRIANGLES
        // We DO need to retain the indices byte offsets for rendering during
        // glDrawElements. We use them as references into an OpenGL buffered
        // array.
        // Each entry is a character with an offset into a indices buffer.
        //                          Count --|     |--- Offset in bytes
        std::unordered_map<char, std::pair<int, GLuint>> indicesPairData{};

        // This matrix is used for text not characters
        Core::Matrix4 model{true};

    public:
        VectorVectorFontAtlas(/* args */);
        ~VectorVectorFontAtlas();

        Core::Vector3 scale{1.0, 1.0, 1.0};
        Core::Vector3 position{0.0, 0.0, 0.0};
        float horzOffset{0.0};

        ErrorConditions initialize(const std::unordered_map<char, std::list<float>> &fontData);
        void constructGeomerty();

        /// @brief Call finalize when all geometry has been added.
        void finalize();

        ErrorConditions configureUniforms() override;
        void constructGeomerty(const std::unordered_map<char, std::list<float>> &fontData);

        void setColor(const std::array<GLfloat, 4> &color);

        void renderChar(char character, const Matrix4 &model);

        /// @brief This overload requires your code to modify the model to adjust for
        ///        character metrics.
        /// @param characters
        /// @param model
        void renderText(const std::list<char> &characters, const Matrix4 &model);
        /// @brief This overload requires you set the class's internal model
        ///        prior to rendering.
        /// @param characters
        void renderText(const std::list<char> &characters);
    };

} // namespace Core
