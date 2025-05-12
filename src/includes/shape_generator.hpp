#pragma once

#include "shape.hpp"
#include "constants.hpp"

namespace Core
{

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
        void generateHLine();
        void generateVLine();
        void generateUnitPlus(); // A "+"
        void generateUnitCircle(int segmentCount, ShapeControls fillType);
        void generateArc(float startAngle, float endAngle, int segmentCount, ShapeControls fillType);
    };

} // namespace Core
