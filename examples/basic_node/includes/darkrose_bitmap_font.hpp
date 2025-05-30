#pragma once

#include <bitmap_font_base.hpp>
#include <color4.hpp>

// Our bitmap font is made up of two triangles forming a rectangle.
// For each 0 pixel we render with a dark color
// For each 1 pixel we render with a light color.
// We use the Rectangle shape from the StaticMonoAtlas.
// In the render call we use a localModel that is composited with the incoming
// model. The localModel has appended a translation equal to 1.0.
// The fontNode has a scale equal to font size.

// Pass in an environment with the font atlas
// use Generator to create PixelCell shape

namespace Game
{
    class DarkroseBitmapFont final : public Core::BitmapFontBase
    {
    private:
        /* data */
        Color4 fgColor{1.0, 0.5, 0.0, 1.0};
        Color4 bgColor{0.2, 0.2, 0.2, 1.0};

    public:
        DarkroseBitmapFont(/* args */) = default;
        ~DarkroseBitmapFont() = default;

        void build() override;
    };

} // namespace Game
