#include <vector_uniform_atlas.hpp>
#include <vector_shape.hpp>
#include <vector_object.hpp>

namespace Core
{
    void VectorUniformAtlas::build(VectorObject *vo)
    {
        buildSquare_(vo);
    }

    void VectorUniformAtlas::buildSquare_(VectorObject *vo)
    {
        VectorShapeSPtr shape = std::make_shared<VectorShape>();
        shape->name = "Square";
        shape->primitiveType = GL_LINE_LOOP;

        int offset = begin();
        shape->setOffset(offset);

        //  top-left         (0,1) +Y
        //                     |
        //                     |
        // -X (-1,0) --------(0,0)-------- (1,0) +X
        //                     |
        //                     |
        //                   (0,-1) -Y      bottom-right
        addVertices({
            0.5f, 0.5f, 0.0f,   // top right
            0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f, // bottom left
            -0.5f, 0.5f, 0.0f   // top left
        });

        addIndex({
            0, 1, 3, // First triangle (upper right)
            1, 2, 3  // Second triangle (lower left)
        });

        shape->vertexCount = end();

        // Top left
        shape->minPoint.x = -0.5f;
        shape->minPoint.y = 0.5f;

        // Bottom right
        shape->maxPoint.x = 0.5f;
        shape->maxPoint.y = -0.5f;

        shapes[shape->name] = shape;
    }

} // namespace Core
