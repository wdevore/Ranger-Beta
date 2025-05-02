#include <atlas.hpp>

namespace Core
{
    void Atlas::addShape(const VectorShapeSPtr &shape)
    {
        shapes_[shape->name] = shape;
    }

} // namespace Core
