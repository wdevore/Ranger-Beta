#include <mesh.hpp>

namespace Core
{

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
