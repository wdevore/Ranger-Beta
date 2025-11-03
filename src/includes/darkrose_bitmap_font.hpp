#pragma once

#include <unordered_map>
#include <vector>

#include <glad/gl.h> // GLuint

#include <bitmap_font_base.hpp>
#include <color4.hpp>
#include <shape_generator.hpp>

// This class generates

// We need to generate vertices and indices.
// What is different is what apps do with geometric shapes.
// Our bitmap font will have a single group of vertices but multiple
// groups of indices for each character in the font.
//
// Typically with geometric shapes we create shapes where each shape
// has vertices and indices. Then add each shape separately to the atlas.
// Once all shapes are added the atlas will iterate each shape and pack
// each vertices an indices into a backing collection

// and once all shapes are added you call a "burn" method which would
// shake and bake them into the atlas. This
namespace Core
{
    /// @brief A gap size of 0.06 works well for a font scaler of 25.0.
    ///
    ///        A gap size of 0.085 works with a scaler of 20.0
    ///
    ///        A gap size of 0.025 works well for larger scalers sizes, for
    ///        example 50.0.
    ///
    ///        If the gap size is to large then each pixel quad becomes smaller
    ///        causing pixel round off by OpenGL which leads to non-uniform
    ///        squares which doesn't look pleasent.
    constexpr float GAPSIZE = 0.06;

    /// @brief A single character is defined as 8*8 byte shape that is
    ///        compacted into a uInt64
    class DarkroseBitmapFont // final : public Core::BitmapFontBase
    {
    private:
        float fontScale{1.0};

        /* data */
        Color4 fgColor{1.0, 0.5, 0.0, 1.0};
        Color4 bgColor{0.2, 0.2, 0.2, 1.0};

        void _printBitmap(std::vector<uint8_t> bytes) const;
        void _printByteAndBitmap(std::vector<uint8_t> bytes) const;

        Core::ShapeGenerator generator{};

        /// @brief An unordered map of characters matched with local-space
        ///        Indices.
        ///
        /// An Atlas should convert them into byte-buffer-space.
        ///                  Group count----|     |------ Group Offset
        std::unordered_map<char, std::pair<int, int>> indicesGroupCounts{};

        std::unordered_map<char, int> charToIndexMap{};

    public:
        DarkroseBitmapFont(/* args */) = default;
        ~DarkroseBitmapFont() = default;

        void build();

        Core::ShapeGenerator getGenerator() { return generator; }
        std::unordered_map<char, std::pair<int, int>> getIndicesGroupData() { return indicesGroupCounts; }

        uint64_t getCharBitmap(char c);
    };
} // namespace Core
