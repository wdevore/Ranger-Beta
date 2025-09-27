#include <cmath>
#include <vector>
#include <iomanip>
// #include <ranges> // for using range loops: requires c++20

#include <glad/gl.h>

#include "shape_generator.hpp"

// ---------- OpenGL unit space -------------------------------
//
//  top-left         (0,1) +Y
//                     |
//                     |
// -X (-1,0) --------(0,0)-------- (1,0) +X
//                     |
//                     |
//                   (0,-1) -Y      bottom-right

namespace Core
{
    constexpr int INDICES_PER_QUAD = 6; // Two triangles

    const std::string GeneratedShapeNames::UnitRectangle = "UnitRectangle";
    const std::string GeneratedShapeNames::UnitTriangle = "UnitTriangle";
    const std::string GeneratedShapeNames::UnitHLine = "UnitHLine";
    const std::string GeneratedShapeNames::UnitVLine = "UnitVLine";
    const std::string GeneratedShapeNames::UnitPlus = "UnitPlus";
    const std::string GeneratedShapeNames::UnitCircle = "UnitCircle";
    const std::string GeneratedShapeNames::UnitArc = "UnitArc";

    const std::string GeneratedShapeNames::BitMapFontX = "BitMapFontChar"; // Debug/Testing
    const std::string GeneratedShapeNames::FontCell = "FontCell";

    void ShapeGenerator::generateUnitRectangle(ShapeControls alignment, ShapeControls fillType)
    {
        shape.clear();
        shape.name = GeneratedShapeNames::UnitRectangle;
        shape.id = nextId++;

        if (alignment == ShapeControls::Centered)
        {
            shape.vertices = {
                0.5f, 0.5f, 0.0f,   // top right
                0.5f, -0.5f, 0.0f,  // bottom right
                -0.5f, -0.5f, 0.0f, // bottom left
                -0.5f, 0.5f, 0.0f   // top left
            };
        }
        else
        {
            // The top-left vertex is at 0,0 and bottom-right is 1,-1
            shape.vertices = {
                1.0f, 0.0f, 0.0f,  // top right
                1.0f, -1.0f, 0.0f, // bottom right
                0.0f, -1.0f, 0.0f, // bottom left
                0.0f, 0.0f, 0.0f,  // top left
            };
        }

        // These indices can be used with the same vertices. You don't need
        // separate vertex arrays.
        if (fillType == ShapeControls::Filled)
        {
            // CCW
            // Technically we could use a quad here.
            shape.indices = {
                0, 3, 1, // First triangle (upper right)
                1, 3, 2  // Second triangle (lower left)
            };
            shape.primitiveMode = GL_TRIANGLES;
        }
        else
        {
            // CCW
            shape.indices = {
                0, 3, 2, 1, // Outside edges
            };
            shape.primitiveMode = GL_LINE_LOOP;
        }
    }

    void ShapeGenerator::generateUnitTriangle(ShapeControls alignment, ShapeControls fillType)
    {
        shape.clear();
        shape.name = GeneratedShapeNames::UnitTriangle;
        shape.id = nextId++;

        if (alignment == ShapeControls::Centered)
        {
            shape.vertices = {
                -0.5, -0.5, 0.0,      // lower-left
                0.5, -0.5, 0.0,       // lower-right
                0.0, M_PI / 6.3, 0.0, // top-center
            };
        }
        else if (alignment == ShapeControls::TopXAxis)
        {
            // The top-left vertex is at 0,0 --> TopXAxis
            shape.vertices = {
                -0.5, -1.0, 0.0, // lower-left
                0.5, -1.0, 0.0,  // lower-right
                0.0, 0.0, 0.0,   // top-center as zero
            };
        }
        else
        {
            shape.vertices = {
                -0.5, 0.0, 0.0, // lower-left as zero
                0.5, 0.0, 0.0,  // lower-right as zero
                0.0, 1.0, 0.0,  // top-center as 1
            };
        }

        // These indices can be used with the same vertices. You don't need
        // separate vertex arrays.
        if (fillType == ShapeControls::Filled)
        {
            // CCW
            // Technically we could use a quad here.
            shape.indices = {
                0, 1, 2, // Single triangle
            };
            shape.primitiveMode = GL_TRIANGLES;
        }
        else
        {
            // CCW
            shape.indices = {
                0, 1, 2, // Outside edges
            };
            shape.primitiveMode = GL_LINE_LOOP;
        }
    }

    void ShapeGenerator::generateUnitHLine()
    {
        shape.clear();
        shape.name = GeneratedShapeNames::UnitHLine;
        shape.id = nextId++;

        shape.vertices = {
            -0.5f, 0.0f, 0.0f, // left
            0.5f, 0.0f, 0.0f,  // right
        };

        shape.indices = {
            0, 1, // Single line
        };

        shape.primitiveMode = GL_LINES;
    }

    void ShapeGenerator::generateUnitVLine()
    {
        shape.clear();
        shape.name = GeneratedShapeNames::UnitVLine;
        shape.id = nextId++;

        shape.vertices = {
            0.0f, -0.5f, 0.0f, // bottom
            0.0f, 0.5f, 0.0f,  // top
        };

        shape.indices = {
            0, 1, // Single line
        };

        shape.primitiveMode = GL_LINES;
    }

    void ShapeGenerator::generateUnitPlus()
    {
        shape.clear();
        shape.name = GeneratedShapeNames::UnitPlus;
        shape.id = nextId++;

        shape.vertices = {
            -0.5, 0.0, 0.0, // left
            0.5, 0.0, 0.0,  // right
            0.0, -0.5, 0.0, // bottom
            0.0, 0.5, 0.0,  // top
        };

        shape.indices = {
            0, 1, 2, 3, // Two lines
        };

        shape.primitiveMode = GL_LINES;
    }

    void ShapeGenerator::generateUnitCircle(int segmentCount, ShapeControls fillType)
    {
        const float radius{0.5}; // = Diameter of 1.0
        const float step{static_cast<float>(M_PI / static_cast<double>(segmentCount))};

        shape.clear();
        shape.name = GeneratedShapeNames::UnitCircle;
        shape.id = nextId++;

        int index{0};

        if (fillType == ShapeControls::Filled)
        {
            // Filled circles have a center point for the Fan fill algorithm
            shape.vertices.push_back(0.0);
            shape.vertices.push_back(0.0);
            shape.vertices.push_back(0.0);

            // The center reference point
            shape.indices.push_back(index++);
            shape.primitiveMode = GL_TRIANGLE_FAN;
        }
        else
        {
            shape.primitiveMode = GL_LINE_LOOP;
        }

        float angle{0.0};
        for (int i = 0; i <= 2 * segmentCount; i++)
        {
            float x = cos(angle) * radius;
            float y = sin(angle) * radius;
            shape.vertices.push_back(x);
            shape.vertices.push_back(y);
            shape.vertices.push_back(0.0);
            shape.indices.push_back(index++);
            angle += step;
        }
    }

    void ShapeGenerator::generateUnitArc(float startAngle, float endAngle, int segmentCount, ShapeControls fillType)
    {
        const float radius{0.5}; // = Diameter of 1.0
        const float step{static_cast<float>((endAngle - startAngle) / static_cast<double>(segmentCount))};

        shape.clear();
        shape.name = GeneratedShapeNames::UnitArc;
        shape.id = nextId++;

        int index{0};

        // Filled circles/arcs have a center point for both the Fan fill algorithm and Arc outline
        shape.vertices.push_back(0.0);
        shape.vertices.push_back(0.0);
        shape.vertices.push_back(0.0);

        // The center reference point
        shape.indices.push_back(index++);

        if (fillType == ShapeControls::Filled)
            shape.primitiveMode = GL_TRIANGLE_FAN;
        else
            shape.primitiveMode = GL_LINE_LOOP;

        for (float angle = 0; angle <= startAngle + endAngle; angle += step)
        {
            float x = cos(angle) * radius;
            float y = sin(angle) * radius;
            shape.vertices.push_back(x);
            shape.vertices.push_back(y);
            shape.vertices.push_back(0.0);
            shape.indices.push_back(index++);
        }
    }

    /// @brief ======== Just for testing ============
    /// @param fillType
    void ShapeGenerator::generateFontChar(uint64_t character, ShapeControls fillType, float gapSize)
    {
        // Use the generator to create the vertices
        generateFontVertices(8, gapSize, ShapeControls::Filled);
        shape.name = GeneratedShapeNames::BitMapFontX; // overwrite

        generateFontIndices(character, shape.indices, indicesOffsets, '!');

        // CCW
        if (fillType == ShapeControls::Filled)
            shape.primitiveMode = GL_TRIANGLES;
        else
            shape.primitiveMode = GL_LINES;
    }

    /// @brief Only generates vertices. Indices still need to be generated by an
    /// outside caller. This produces 256 vertices with the typical parameters.
    /// @param numberOfSquaresPerSide is typically 8
    /// @param gapSize is typically 0.025
    void ShapeGenerator::generateFontVertices(int numberOfSquaresPerSide,
                                              float gapSize,
                                              ShapeControls fillType)
    {
        shape.clear();
        shape.name = GeneratedShapeNames::FontCell;
        shape.id = nextId++;

        const float unitLength{1.0F};
        const int numberOfGaps = numberOfSquaresPerSide - 1;
        const float sideLenth =
            (unitLength - (numberOfGaps * gapSize)) / numberOfSquaresPerSide; // s

        float my{0.5}; // Initial start value
        float y1 = my; // Start y position
        float y2{0.0}; // End y position

        float y{0.0};
        bool evenRow{true};

        for (int row = 0; row < numberOfSquaresPerSide * 2; row++)
        {
            // std::cout << "row: " << row << std::endl;
            if (evenRow)
            {
                my -= sideLenth;
            }
            y2 = my;
            y = y1;

            // std::cout << "Y vertex from: " << y1 << " to: " << y2 << std::endl;
            // A side Edge doesn't have a gap
            if (!evenRow)
            {
                my -= gapSize;
            }
            y1 = my;

            evenRow = !evenRow;

            float mx{-0.5}; // Initial start value
            float x1 = mx;  // Start X position
            float x2{0.0};  // End x position

            for (int col = 0; col < numberOfSquaresPerSide; col++)
            {
                // std::cout << "col: " << col << std::endl;
                mx += sideLenth;
                x2 = mx;

                // std::cout << "X vertex from: " << x1 << " to: " << x2 << std::endl;
                // ------- Output ----------------
                // std::cout << " Horizontal Edge: (" << x1 << "," << y << ")-(" << x2 << "," << y << ")" << std::endl;
                shape.vertices.push_back(x1);
                shape.vertices.push_back(y);
                shape.vertices.push_back(0.0);

                shape.vertices.push_back(x2);
                shape.vertices.push_back(y);
                shape.vertices.push_back(0.0);

                // A side Edge doesn't have a gap
                if (col < numberOfGaps)
                    mx += gapSize;
                x1 = mx;
            }
        }

        shape.primitiveMode = fillType == ShapeControls::Filled ? GL_TRIANGLES : GL_LINE_LOOP;
    }

    int ShapeGenerator::generateFontIndices(uint64_t char8x8,
                                            std::vector<GLuint> &indices,
                                            std::unordered_map<char, std::pair<int, int>> &indicesGroupCounts,
                                            char charSymbol)
    {
        // std::ios_base::fmtflags originalFlags = std::cout.flags();
        // std::cout << "char '" << charSymbol << "': 0x" << std::hex << std::setw(16) << std::setfill('0') << char8x8 << std::dec << std::endl;

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
        // char darkChar = BitmapFonts::darkRose_chars[charIndex];
        // indicesOffsets[charSymbol] = indices.size();

        // Shift scaler for bit positioning
        int shiftPosition{0};

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
            uint8_t byte = (char8x8 >> shift) & 0xFF;
            bytes[byteIndex] = byte;
            // std::cout << "byte: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << std::endl;
            shiftPosition++;
        }
        // std::cout << std::endl;

        // std::cout.flags(originalFlags);

        // _printByteAndBitmap(bytes);

        int idx_row{0}; // Row is vertical (bytes)
        int idx_col{0}; // Column is horizontal (bits)

        int indexGroupCount{0};
        int indexGroupOffset = indices.size();

        // generator.indicesOffsets[darkChar] = indexGroupOffset;

        // for (uint8_t byte_row : std::ranges::views::reverse(bytes)) // requires c++20

        // for (auto &&byte_row : bytes) // This is the forward direction and the
        // character is upside down. If you want to iterate in this direction then
        // you will need to use a negative Y axis scaler to flip the character (not recommeded)
        // for example: modelR.scaleBy(50.0, -50.0, 1.0);

        // Now we take each byte in the array and iterate through each bit
        // in the byte--in reverse: std::vector<uint8_t>::reverse_iterator
        for (auto it = bytes.rbegin(); it != bytes.rend(); ++it)
        {
            uint8_t byte_row = *it;

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
                    getSubSquareEboIndices(idx_col, idx_row, SQUARES_PRE_SIDE, indices);
                    indexGroupCount += INDICES_PER_QUAD;
                }
                idx_col++; // Move to next bit column
            }

            idx_row++; // Move to next byte row
        }

        // indexGroupOffset += indexGroupCount;

        // Move to next char label.
        // charIndex++;

        // #elif defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#endif

        //                     Group count----|                    |------ Group Offset
        indicesGroupCounts[charSymbol] = {indexGroupCount, indexGroupOffset};

        return indexGroupCount;
    }

    void ShapeGenerator::getSubSquareEboIndices(int grid_x, int grid_y, int squaresPerSide, std::vector<GLuint> &out_indices)
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

    Rectangle ShapeGenerator::generateABBox(const Shape &shape)
    {
        Rectangle rect{};
        rect.set(shape.vertices);
        return rect;
    }

    Rectangle ShapeGenerator::generateABBox(const std::vector<GLfloat> &vertices)
    {
        Rectangle rect{};
        rect.set(vertices);
        return rect;
    }

    /// @brief Only call this method immediately after calling on of the generator
    /// methods.
    /// @return
    Rectangle ShapeGenerator::generateABBox()
    {
        Rectangle rect{};
        rect.set(shape.vertices);
        return rect;
    }

    /// @brief Only call this method immediately after calling on of the generator
    /// methods.
    /// @return
    void ShapeGenerator::generateABBox(Rectangle &box)
    {
        box.set(shape.vertices);
    }
} // namespace Core
