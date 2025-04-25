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
        // Make this Vbo active
        glBindBuffer(GL_ARRAY_BUFFER, vboId_);
        // copy our vertices array in a buffer for OpenGL to use
        glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(GLfloat), &mesh->vertices[0], GL_STATIC_DRAW);
    }

} // namespace Core
