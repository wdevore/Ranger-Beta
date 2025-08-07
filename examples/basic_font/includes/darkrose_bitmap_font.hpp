#pragma once

#include <map>

#include <bitmap_font_base.hpp>
#include <color4.hpp>

namespace Game
{
    class DarkroseBitmapFont final : public Core::BitmapFontBase
    {
    private:
        /* data */
        Color4 fgColor{1.0, 0.5, 0.0, 1.0};
        Color4 bgColor{0.2, 0.2, 0.2, 1.0};
        // Map of <string, int-offset>
        std::map<std::string, int> indicesOffsets{};

    public:
        DarkroseBitmapFont(/* args */) = default;
        ~DarkroseBitmapFont() = default;

        void build(Core::StaticMonoAtlas &atlas) override;

        /// @brief Calculates the 6 EBO indices for a specific square in a grid.
        ///
        /// @param grid_x The 0-indexed column of the target square.
        /// @param grid_y The 0-indexed row of the target square.
        /// @param squaresPerSide The number of squares along one side of the grid (e.g., 8 for an 8x8 grid).
        /// @param out_indices A reference to a std::vector<GLuint> where the 6 calculated indices will be stored.
        ///                    The vector will be cleared before adding new indices.
        /// @throws std::out_of_range if grid_x or grid_y are out of bounds
        void getSubSquareEboIndices(
            int grid_x,
            int grid_y,
            int squaresPerSide,
            std::vector<GLuint> &out_indices);
    };

} // namespace Game
