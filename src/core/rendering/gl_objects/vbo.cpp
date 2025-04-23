#include <iostream>

#include <vbo.hpp>
#include <mesh.hpp>

namespace Core
{
    Vbo::~Vbo()
    {
        if (genBound_)
            glDeleteBuffers(1, &vboId_);
        std::cout << "~VBO" << std::endl;
    }

    void Vbo::gen()
    {
        if (!genBound_)
        {
            glGenBuffers(1, &vboId_);
            genBound_ = true;
        }
    }

    void Vbo::bind(Mesh *mesh)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vboId_);
        glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(GLfloat), &mesh->vertices[0], GL_STATIC_DRAW);
    }

} // namespace Core
