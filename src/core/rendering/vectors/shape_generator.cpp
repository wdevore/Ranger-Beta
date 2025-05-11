#include <glad/gl.h>

#include "shape_generator.hpp"

namespace Core
{
    void ShapeGenerator::generateUnitRectangle(ShapeControls alignment, ShapeControls fillType)
    {
        shape.clear();
        shape.name = "UnitRectangle";
        shape.id = nextId++;

        //  top-left         (0,1) +Y
        //                     |
        //                     |
        // -X (-1,0) --------(0,0)-------- (1,0) +X
        //                     |
        //                     |
        //                   (0,-1) -Y      bottom-right
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

} // namespace Core
