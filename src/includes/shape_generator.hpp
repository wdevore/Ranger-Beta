#pragma once
#include <string>

#include "shape.hpp"
#include "constants.hpp"
#include "rectangle.hpp"

namespace Core
{

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

        // These shape generators define vertices and indices that represent
        // unit-sized vector shapes. These shapes are then loaded into an Atlas.

        void generateUnitRectangle(ShapeControls alignment, ShapeControls fillType);
        void generateUnitTriangle(ShapeControls alignment, ShapeControls fillType);
        void generateUnitHLine();
        void generateUnitVLine();
        void generateUnitPlus(); // A "+"
        void generateUnitCircle(int segmentCount, ShapeControls fillType);
        void generateUnitArc(float startAngle, float endAngle, int segmentCount, ShapeControls fillType);

        void generateFontCells(int numberOfSquaresPerSide, float gapSize, ShapeControls fillType);

        Rectangle generateABBox();
        Rectangle generateABBox(const Shape &shape);
        Rectangle generateABBox(const std::vector<GLfloat> &vertices);
        void generateABBox(Rectangle &box);
    };

} // namespace Core
