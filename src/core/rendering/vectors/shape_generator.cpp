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

    void ShapeGenerator::generateHLine()
    {
        shape.clear();
        shape.name = "UnitHLine";
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

    void ShapeGenerator::generateVLine()
    {
        shape.clear();
        shape.name = "UnitVLine";
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
        shape.name = "UnitPlus";
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
        shape.name = "UnitCircle";
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

    void ShapeGenerator::generateArc(float startAngle, float endAngle, int segmentCount, ShapeControls fillType)
    {
        const float radius{0.5}; // = Diameter of 1.0
        const float step{static_cast<float>((endAngle - startAngle) / static_cast<double>(segmentCount))};

        shape.clear();
        shape.name = "UnitArc";
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
} // namespace Core
