#include "shape.hpp"

namespace Core
{
    void Shape::clear()
    {
        id = 0;
        name = "";
        dirty = false;
        vertices.clear();
        indices.clear();
        vertexOffset = 0;
        indicesOffset = 0;
        indicesCount = 0;
        primitiveMode = 0;
    }
} // namespace Core
