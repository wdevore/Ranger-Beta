#include <vector_object.hpp>

namespace Core
{
    void VectorObject::construct()
    {
        atlas.construct(this);
        vao.construct();
        vao.bind(&atlas.mesh);
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
        // vao.bind(&atlas.mesh);
    }

    int VectorObject::begin()
    {
        return atlas.begin();
    }

} // namespace Core
