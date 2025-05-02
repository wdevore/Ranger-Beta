#include <iostream>

#include <vao.hpp>
#include <mesh.hpp>

// A vertex array object (also known as VAO) can be bound just like a vertex
// buffer object and any subsequent vertex attribute calls from that point on
// will be stored inside the VAO. This has the advantage that when configuring
// vertex attribute pointers you only have to make those calls once and whenever
// we want to draw the object, we can just bind the corresponding VAO.
// This makes switching between different vertex data and attribute
// configurations as easy as binding a different VAO. All the state we just
// set is stored inside the VAO.

// A vertex array object stores the following:
//
// - Calls to glEnableVertexAttribArray or glDisableVertexAttribArray.
// - Vertex attribute configurations via glVertexAttribPointer.
// - Vertex buffer objects associated with vertex attributes by calls to
//   glVertexAttribPointer.

namespace Core
{
    void Vao::construct()
    {
    }

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

        // Any subsequent vertex attribute calls from this point on
        // will be stored inside the VAO.
        // From this point on we should bind/configure the corresponding VBO(s)
        // and attribute pointer(s) and then unbind the VAO for later use.
        mesh->bind();

        // then set our vertex attributes pointers
        glVertexAttribPointer(0, XYZ_Component_count, GL_FLOAT, GL_FALSE, XYZ_Component_count * sizeof(GLfloat), (GLvoid *)0);

        glEnableVertexAttribArray(0);

        // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound
        // vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs),
        // remember: do NOT unbind the EBO, keep it bound to this VAO
        glBindVertexArray(0);
    }

    void Vao::use()
    {
        glBindVertexArray(vaoId_);
    }

    void Vao::unUse()
    {
        // See opengl wiki as to why "glBindVertexArray(0)" isn't really necessary here:
        // https://www.opengl.org/wiki/Vertex_Specification#Vertex_Buffer_Object
        // Note the line "Changing the GL_ARRAY_BUFFER binding changes nothing about vertex attribute 0..."
        glBindVertexArray(0);
    }

    void Vao::release()
    {
    }

} // namespace Core
