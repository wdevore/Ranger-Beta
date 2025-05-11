#include <cmath>

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
    void ShapeGenerator::generateUnitRectangle(ShapeControls alignment, ShapeControls fillType)
    {
        shape.clear();
        shape.name = "UnitRectangle";
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
        shape.name = "UnitTriangle";
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

} // namespace Core
