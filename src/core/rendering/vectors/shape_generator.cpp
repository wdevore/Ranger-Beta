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
    const std::string GeneratedShapeNames::UnitRectangle = "UnitRectangle";
    const std::string GeneratedShapeNames::UnitTriangle = "UnitTriangle";
    const std::string GeneratedShapeNames::UnitHLine = "UnitHLine";
    const std::string GeneratedShapeNames::UnitVLine = "UnitVLine";
    const std::string GeneratedShapeNames::UnitPlus = "UnitPlus";
    const std::string GeneratedShapeNames::UnitCircle = "UnitCircle";
    const std::string GeneratedShapeNames::UnitArc = "UnitArc";

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

    /// @brief Only generates vertices. Indices still need to be generated by an
    /// outside caller. This produces 256 vertices with the typical parameters.
    /// @param numberOfSquaresPerSide is typically 8
    /// @param gapSize is typically 0.025
    void ShapeGenerator::generateFontCells(int numberOfSquaresPerSide, float gapSize, ShapeControls fillType)
    {
        shape.clear();
        shape.name = GeneratedShapeNames::FontCell;
        shape.id = nextId++;

        const float unitLength{1.0};
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
