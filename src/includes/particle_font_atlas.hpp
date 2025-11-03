#pragma once

#include <list>
#include <utility> // std::pair
#include <unordered_map>

#include "atlas.hpp"
#include "shape.hpp"
#include <shape_generator.hpp>
#include <darkrose_bitmap_font.hpp>
#include "matrix4.hpp"
#include "vector3.hpp"

namespace Core
{
    class ParticleFontAtlas final : public Atlas
    {
    private:
        GLint colorLoc{};
        GLint particleSizeLoc{};
        // const std::string uniColor{"fragColor"};
        const std::string uniColor{"textColor"};
        const std::string uniPointSize{"pointSize"};

        ErrorConditions initialAndBuildShader_();

        // Each entry is a character with a vertex offset and count.
        //                          Offset --|     |--- Count
        std::unordered_map<char, std::pair<int, int>> charVertexData{};

        Core::DarkroseBitmapFont bitmapFont{};
        GLenum primitiveMode{};

        Core::Matrix4 model{true};

    public:
        ParticleFontAtlas();
        ~ParticleFontAtlas();

        Core::Vector3 scale{1.0, 1.0, 1.0};
        Core::Vector3 position{0.0, 0.0, 0.0};
        float horzOffset{0.0};
        float particleSize{1.0f};

        ErrorConditions initialize();
        void constructGeomerty();

        void finalize();

        ErrorConditions configureUniforms() override;

        void setColor(const std::array<GLfloat, 4> &color);
        void setParticleSize(float size);

        void renderChar(char character, const Matrix4 &model);
        void renderText(const std::list<char> &characters, const Matrix4 &model);
        void renderText(const std::list<char> &characters);
    };

} // namespace Core