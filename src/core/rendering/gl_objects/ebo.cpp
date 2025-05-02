#include <iostream>

#include <vbo.hpp>
#include <mesh.hpp>

namespace Core
{

    Ebo::~Ebo()
    {
        if (genBound_)
            glDeleteBuffers(1, &eboId_);
        std::cout << "~Ebo" << std::endl;
    }

    void Ebo::construct()
    {
    }

    void Ebo::gen()
    {
        if (!genBound_)
        {
            glGenBuffers(1, &eboId_);
            genBound_ = true;
        }
    }

    void Ebo::bind(Mesh *mesh)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(GLuint), &mesh->indices[0], GL_STATIC_DRAW);
    }

} // namespace Core
