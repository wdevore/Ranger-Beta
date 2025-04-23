#include <iostream>

#include <vao.hpp>
#include <mesh.hpp>

namespace Core
{
    Vao::~Vao()
    {
        if (genBound_)
            glDeleteVertexArrays(1, &vaoId_);
        std::cout << "~VAO" << std::endl;
    }

    void Vao::bind(Mesh *mesh)
    {
        if (!genBound_)
        {
            glGenVertexArrays(1, &vaoId_);
            genBound_ = true;
        }

        mesh->gen();

        // Bind the Vertex Array Object first, then bind and set vertex buffer(s)
        // and attribute pointer(s).
        glBindVertexArray(vaoId_);

        mesh->bind();

        glVertexAttribPointer(0, XYZ_Component_count, GL_FLOAT, GL_FALSE, XYZ_Component_count * sizeof(GLfloat), (GLvoid *)0);

        glEnableVertexAttribArray(0);

        // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound
        // vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs),
        // remember: do NOT unbind the EBO, keep it bound to this VAO
        glBindVertexArray(0);
    }

} // namespace Core
