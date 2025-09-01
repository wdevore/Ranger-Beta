#include <cstdint>
#include <iostream>
#include <iomanip> // Required for std::setw and std::setfill
#include <bitset>  // For std::bitset

#include <darkrose_bitmap_font.hpp>
#include <dark_rose.hpp>
#include <shape_generator.hpp>

namespace Game
{
    void DarkroseBitmapFont::build()
    {
        Core::ShapeGenerator generator{};

        // --------------------------------------------------------
        // -------- Generate Vertices -----------------------------
        // --------------------------------------------------------
        generator.generateFontCells(SQUARES_PRE_SIDE, GAPSIZE, Core::ShapeControls::Filled);

        // Now that we have a Shape with ONLY vertices we need to build
        // indices groups for each character in the font.
        // In the end the shape will have a continous set of indices broken into
        // groups one for each character. However, we need to track the start
        // of each group, and later when the indices are mapped into the
        // byte-buffer space we map the group offsets as well.

        // --------------------------------------------------------
        // -------- Generate Indices ------------------------------
        // --------------------------------------------------------
        int charIndex = {0};
        for (auto &&character : BitmapFonts::darkRose_font)
        {
            // std::ios_base::fmtflags originalFlags = std::cout.flags();
            // std::cout << "char: 0x" << std::hex << std::setw(16) << std::setfill('0') << character << std::dec << std::endl;
#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

            // Little-endian systems store the LSB at the lowest memory address.
            // To get bytes in LSB to MSB order:
            // For example: "0x0808080800080000"
            //                 |              |
            //           (last) \MSB           \LSB (first)
            std::vector<uint8_t> bytes;
            bytes.resize(SQUARES_PRE_SIDE);

            // Capture group offset for current character.
            // As we create indices groups we record start of the offset which
            // is based on the current "size" of the indices count.
            indicesOffsets[BitmapFonts::darkRose_chars[charIndex]] = generator.shape.indices.size();

            // Shift scaler for bit positioning
            int shiftPosition = 0;

            // ------------------------------
            // Break uInt64 into bytes.
            // ------------------------------
            for (int byteIndex = BYTES - 1; byteIndex >= 0; byteIndex--)
            {
                // For byte 0 (LSB), shift by 0 bits
                // For byte 1, shift by 8 bits
                // ...
                // For byte 7 (MSB), shift by 56 bits
                int shift = shiftPosition * (BITS + 1);
                // Isolate the byte
                uint8_t byte = (character >> shift) & 0xFF;
                bytes[byteIndex] = byte;
                // std::cout << "byte: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << std::endl;
                shiftPosition++;
            }
            // std::cout << std::endl;

            // std::cout.flags(originalFlags);

            // _printByteAndBitmap(bytes);

            int idx_row = 0; // Row is vertical (bytes)
            int idx_col = 0; // Column is horizontal (bits)

            // Now we take each byte in the array and iterate through each bit
            // in the byte.
            for (auto &&byte_row : bytes)
            {
                idx_col = 0;
                // Iterate on each column bit
                for (int grid_col = BITS; grid_col >= 0; grid_col--)
                {
                    // isolate bit.
                    int bit = (byte_row >> grid_col) & 1;

                    if (bit == 1) // Find a pixel that is lit.
                    {
                        // std::cout << "(Col: " << idx_col << ", Row:" << idx_row << ")" << std::endl;
                        // Add indices Quad
                        // Note: These indices are local-space and will eventually
                        // be mapped to byte-buffer-space.
                        _getSubSquareEboIndices(idx_col, idx_row, SQUARES_PRE_SIDE, generator.shape.indices);
                    }
                    idx_col++; // Move to next bit column
                }

                idx_row++; // Move to next byte row
            }

            // Move to next char label.
            charIndex++;

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

    void DarkroseBitmapFont::_getSubSquareEboIndices(int grid_x, int grid_y, int squaresPerSide, std::vector<GLuint> &out_indices)
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

        //   TL      TR
        //   .-------.
        //   | \ 1st |
        //   |  \    |
        //   |    \  |
        //   | 2nd \ |
        //   .-------.
        //  BL       BR
        // std::cout << "TL--TR" << std::endl;
        // std::cout
        //     << std::dec << std::setw(3) << std::setfill('0') << idx_TL << " "
        //     << std::dec << std::setw(3) << std::setfill('0') << idx_TR
        //     << std::endl;
        // std::cout << "BL--BR" << std::endl;
        // std::cout
        //     << std::dec << std::setw(3) << std::setfill('0') << idx_BL << " "
        //     << std::dec << std::setw(3) << std::setfill('0') << idx_BR
        //     << std::endl;

        // std::cout << "TR  TL  BR  : BR  TL  BL" << std::endl;
        // std::cout << std::dec << std::setw(3) << std::setfill('0') << idx_TR << " "
        //           << std::dec << std::setw(3) << std::setfill('0') << idx_TL << " "
        //           << std::dec << std::setw(3) << std::setfill('0') << idx_BR << "   "
        //           << std::dec << std::setw(3) << std::setfill('0') << idx_BR << " "
        //           << std::dec << std::setw(3) << std::setfill('0') << idx_TL << " "
        //           << std::dec << std::setw(3) << std::setfill('0') << idx_BL
        //           << std::endl;

        // out_indices.clear();   <-- oops
        // out_indices.reserve(6);

        // Add indices for the first triangle (TR, TL, BR) - CCW
        out_indices.push_back(idx_TR);
        out_indices.push_back(idx_TL);
        out_indices.push_back(idx_BR);

        // Add indices for the second triangle (BR, TL, BL) - CCW
        out_indices.push_back(idx_BR);
        out_indices.push_back(idx_TL);
        out_indices.push_back(idx_BL);
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
} // namespace Game
