#pragma once

#include <vector>

#include <glad/gl.h>
// We don't need to include opengl <gl.h> because glfw does it already.
#include <GLFW/glfw3.h>

#include <vbo.hpp>
#include <ebo.hpp>

// A Mesh is given to a Vao during binding. The Vao will then call this
// Mesh's bind method to further bind the Vbo and Ebo during the Vao's
// binding step. The Vao is container like concept around Vbo and Ebo's.
namespace Core
{
    class Mesh
    {
    private:
        /* data */
    public:
        Vbo vbo{};
        Ebo ebo{};
        std::vector<GLfloat> vertices{};
        std::vector<GLuint> indices{};

        Mesh(/* args */) = default;
        ~Mesh() = default;

        void construct();
        void gen();
        void bind();
    };

} // namespace Core
