#include <cstdint>
#include <vector>
#include <iostream>
#include <iomanip> // Required for std::setw and std::setfill

#include <darkrose_bitmap_font.hpp>
#include <dark_rose.hpp>
#include <shape_generator.hpp>

namespace Game
{
    void DarkroseBitmapFont::build(Core::StaticMonoAtlas &atlas)
    {

        Core::ShapeGenerator generator{};
        generator.generateFontCells(8, 0.025, Core::ShapeControls::Filled);

        // Now that we have a Shape with ONLY vertices we need to build
        // indices blocks for each character in the font.
        // In the end the shape will have a continous set of indices.

        // getSubSquareEboIndices(0, 1, 8, generator.shape.indices);

        // As we create index blocks we record there offsets.

        // First get the current offset in the atlas.

        for (auto &&character : BitmapFonts::darkRose_font)
        {
            std::cout << "character: 0x" << std::hex << character << std::dec << std::endl;
#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

            // Little-endian systems store the LSB at the lowest memory address.
            // To get bytes in LSB to MSB order:
            std::vector<uint8_t> bytes_lsb_first;
            bytes_lsb_first.resize(8);

            for (int i = 0; i < 8; ++i)
            {
                // For byte 0 (LSB), shift by 0 bits
                // For byte 1, shift by 8 bits
                // ...
                // For byte 7 (MSB), shift by 56 bits
                int shift = i * 8;
                uint8_t byte = (character >> shift) & 0xFF;
                bytes_lsb_first[i] = byte;
                // std::cout << "byte: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << std::endl;
            }
            // std::cout << "byte: 0x" << std::dec << std::endl;

#elif defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
            // Big
            std::cout << "Big Endian" << std::endl;
            // A vector to store the bytes
            std::vector<uint8_t> bytes;
            bytes.resize(8); // A 64-bit number has 8 bytes

            // Extract bytes from most significant (byte 7) to least significant (byte 0)
            // Or you can think of it as byte 0 to byte 7 if you prefer left-to-right reading of the hex.
            // The loop below extracts them in the order they appear if you read the hex string.
            for (int i = 0; i < 8; ++i)
            {
                // Shift the bits to the right so the byte we want is in the LSB position
                // For byte 7 (MSB), shift by (7-0)*8 = 56 bits
                // For byte 6, shift by (7-1)*8 = 48 bits
                // ...
                // For byte 0 (LSB), shift by (7-7)*8 = 0 bits
                uint8_t byte = (character >> (56 - (i * 8))) & 0xFF;
                bytes[i] = byte;
            }
#endif
        }
    }

    void DarkroseBitmapFont::getSubSquareEboIndices(int grid_x, int grid_y, int squaresPerSide, std::vector<GLuint> &out_indices)
    {
        if (grid_x < 0 || grid_x >= squaresPerSide || grid_y < 0 || grid_y >= squaresPerSide)
        {
            throw std::out_of_range("grid_x or grid_y is out of bounds for the given squaresPerSide.");
        }

        // Calculate the four vertex indices for the square (grid_x, grid_y)
        // N is used for brevity in the formulas.
        const int N = squaresPerSide;

        // Top-Left vertex index
        GLuint idx_TL = static_cast<GLuint>((grid_y * 2 * N + grid_x) * 2);
        // Top-Right vertex index
        GLuint idx_TR = idx_TL + 1;

        // Bottom-Left vertex index
        GLuint idx_BL = static_cast<GLuint>(((grid_y * 2 + 1) * N + grid_x) * 2);
        // Bottom-Right vertex index
        GLuint idx_BR = idx_BL + 1;

        out_indices.clear();
        out_indices.reserve(6);

        // Add indices for the first triangle (TR, TL, BR) - CCW
        out_indices.push_back(idx_TR);
        out_indices.push_back(idx_TL);
        out_indices.push_back(idx_BR);

        // Add indices for the second triangle (BR, TL, BL) - CCW
        out_indices.push_back(idx_BR);
        out_indices.push_back(idx_TL);
        out_indices.push_back(idx_BL);
    }
} // namespace Game
