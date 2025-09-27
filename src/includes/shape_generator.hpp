#pragma once
#include <string>
#include <unordered_map>

#include "shape.hpp"
#include "constants.hpp"
#include "rectangle.hpp"

namespace Core
{
    constexpr int SQUARES_PRE_SIDE = 8;
    constexpr int BYTES = 8;
    constexpr int BITS = 8 - 1;

    struct GeneratedShapeNames
    {
        static const std::string UnitRectangle;
        static const std::string UnitTriangle;
        static const std::string UnitHLine;
        static const std::string UnitVLine;
        static const std::string UnitPlus;
        static const std::string UnitCircle;
        static const std::string UnitArc;
        static const std::string FontCell;

        // Simple Bitmap font test
        static const std::string BitMapFontX;
    };

    class ShapeGenerator
    {
    private:
        /* data */
        int nextId{0};

    public:
        Shape shape{};

        ShapeGenerator(/* args */) = default;
        ~ShapeGenerator() = default;

        std::unordered_map<char, std::pair<int, int>> indicesOffsets{};

        // These shape generators define vertices and indices that represent
        // unit-sized vector shapes. These shapes are then loaded into an Atlas.

        void generateUnitRectangle(ShapeControls alignment, ShapeControls fillType);
        void generateUnitTriangle(ShapeControls alignment, ShapeControls fillType);
        void generateUnitHLine();
        void generateUnitVLine();
        void generateUnitPlus(); // A "+"
        void generateUnitCircle(int segmentCount, ShapeControls fillType);
        void generateUnitArc(float startAngle, float endAngle,
                             int segmentCount, ShapeControls fillType);

        void generateFontChar(uint64_t character, ShapeControls fillType, float gapSize);
        void generateFontVertices(int numberOfSquaresPerSide,
                                  float gapSize, ShapeControls fillType);
        int generateFontIndices(uint64_t char8x8,
                                std::vector<GLuint> &indices,
                                //                   Group count----|    |------ Group Offset
                                std::unordered_map<char, std::pair<int, int>> &indicesOffsets,
                                char charSymbol);
        void getSubSquareEboIndices(int grid_x, int grid_y,
                                    int squaresPerSide,
                                    std::vector<GLuint> &out_indices);

        Rectangle generateABBox();
        Rectangle generateABBox(const Shape &shape);
        Rectangle generateABBox(const std::vector<GLfloat> &vertices);
        void generateABBox(Rectangle &box);
    };

} // namespace Core
