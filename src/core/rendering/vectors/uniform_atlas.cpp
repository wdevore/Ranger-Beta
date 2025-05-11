#include <glad/gl.h>

#include <uniform_atlas.hpp>
#include <vector_shape.hpp>
#include <vector_object.hpp>

namespace Core
{
    void UniformAtlas::buildSquare_(VectorObject *vo)
    {
        VectorShapeSPtr shape = std::make_shared<VectorShape>();
        shape->name = "Square";
        shape->primitiveType = GL_LINE_LOOP;

        shape->setOffset(vo->atlas.begin());

        vo->atlas.add(0.0f, 0.0f);
        vo->atlas.add(1.0f, 0.0f);
        vo->atlas.add(1.0f, 1.0f);
        vo->atlas.add(0.0f, 1.0f);

        shape->vertexCount = vo->atlas.end();

        shape->minPoint.x = 0.0f;
        shape->minPoint.y = 0.0f;

        shape->maxPoint.x = 1.0f;
        shape->maxPoint.y = 1.0f;

        addShape(shape);
    }

    void Core::UniformAtlas::construct(VectorObject *vo)
    {
        buildSquare_(vo);
    }

} // namespace Core
