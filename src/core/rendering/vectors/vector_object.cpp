#include <vector_object.hpp>

namespace Core
{
    void VectorObject::construct()
    {
    }

    void VectorObject::use()
    {
        vao.use();
    }

    void VectorObject::unUse()
    {
        vao.unUse();
    }

    void VectorObject::release()
    {
        vao.release();
    }

    void VectorObject::bind()
    {
        vao.bind(&atlas.mesh);
    }

} // namespace Core
