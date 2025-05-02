#include <mesh.hpp>

namespace Core
{
    void Mesh::construct()
    {
        vbo.construct();
        ebo.construct();
    }

    void Mesh::gen()
    {
        vbo.gen();
    }

    void Mesh::bind()
    {
        vbo.bind(this);
        ebo.bind(this);
    }

} // namespace Core
