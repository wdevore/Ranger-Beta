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

        void generateUnitRectangle(ShapeControls alignment, ShapeControls fillType);
    };

} // namespace Core
