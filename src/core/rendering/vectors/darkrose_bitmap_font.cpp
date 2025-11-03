#include <cstdint>
#include <iostream>
#include <iomanip> // Required for std::setw and std::setfill
#include <bitset>  // For std::bitset

#include <darkrose_bitmap_font.hpp>
#include <dark_rose.hpp>
#include <shape_generator.hpp>

namespace Core
{
    void DarkroseBitmapFont::build()
    {
        // --------------------------------------------------------
        // -------- Generate Vertices -----------------------------
        // --------------------------------------------------------
        generator.generateFontVertices(Core::SQUARES_PRE_SIDE, GAPSIZE, Core::ShapeControls::Filled);

        // Now that we have a Shape with ONLY vertices we need to build
        // indices groups for each character in the font.
        // In the end the shape will have a continous set of indices broken into
        // groups one for each character. However, we need to track the start
        // of each group, and later when the indices are mapped into the
        // byte-buffer space we map the group offsets as well.

        // --------------------------------------------------------
        // -------- Generate Indices ------------------------------
        // --------------------------------------------------------
        int charIndex{0};

        // By reserving memory we improve efficiency and minimize any kind
        // scrambling by the allocator. I shouldn't rely on this strategy.
        indicesGroupCounts.reserve(DarkroseFont::darkRose_font.size());
        charToIndexMap.reserve(DarkroseFont::darkRose_chars.size());

        for (int i = 0; i < DarkroseFont::darkRose_chars.size(); ++i)
        {
            charToIndexMap[DarkroseFont::darkRose_chars[i]] = i;
        }

        for (auto &&char8x8 : DarkroseFont::darkRose_font)
        {
            char charSymbol = DarkroseFont::darkRose_chars[charIndex];

            generator.generateBitmapFontIndices(char8x8,
                                                generator.shape.indices,
                                                indicesGroupCounts,
                                                charSymbol);

            charIndex++;
        }
    }

    uint64_t DarkroseBitmapFont::getCharBitmap(char c)
    {
        auto it = charToIndexMap.find(c);
        if (it != charToIndexMap.end())
        {
            return DarkroseFont::darkRose_font[it->second];
        }

        return 0; // Return 0 for characters not in the font
    }

    void DarkroseBitmapFont::_printBitmap(std::vector<uint8_t> bytes) const
    {
        for (auto &&byte : bytes)
        {
            std::cout << "byte: " << std::bitset<8>(byte) << std::endl;
        }
    }

    void DarkroseBitmapFont::_printByteAndBitmap(std::vector<uint8_t> bytes) const
    {
        std::cout << "           01234567" << std::endl;
        for (auto &&byte : bytes)
        {
            std::cout << "byte: " << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << " = " << std::bitset<8>(byte) << std::endl;
        }
    }
} // namespace Core
