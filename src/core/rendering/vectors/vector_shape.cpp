
#include <vector_shape.hpp>

namespace Core
{
    int VectorShape::offset() const
    {
        return offset_;
    }

    void VectorShape::setOffset(int offset)
    {
        offset_ = offset * sizeof(unsigned int);
    }

} // namespace Core
