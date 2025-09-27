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
namespace Game
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
    class DarkroseBitmapFont final : public Core::BitmapFontBase
    {
    private:
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

    public:
        DarkroseBitmapFont(/* args */) = default;
        ~DarkroseBitmapFont() = default;

        void build() override;

        Core::ShapeGenerator getGenerator() override { return generator; }
        std::unordered_map<char, std::pair<int, int>> getIndicesGroupCounts() override { return indicesGroupCounts; }
    };

    static void generateIndices(uint64_t char8x8,
                                std::vector<GLuint> &indices,
                                std::unordered_map<char, int> &indicesOffsets);

    /// @brief Calculates the 6 EBO indices for a specific square in a grid.
    ///
    /// @param grid_x The 0-indexed column of the target square.
    /// @param grid_y The 0-indexed row of the target square.
    /// @param squaresPerSide The number of squares along one side of the grid (e.g., 8 for an 8x8 grid).
    /// @param out_indices A reference to a std::vector<GLuint> where the 6 calculated indices will be stored.
    ///                    The vector will be cleared before adding new indices.
    /// @throws std::out_of_range if grid_x or grid_y are out of bounds
    static void getSubSquareEboIndices(
        int grid_x,
        int grid_y,
        int squaresPerSide,
        std::vector<GLuint> &out_indices);

} // namespace Game
