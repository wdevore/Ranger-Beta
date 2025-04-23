#pragma once

#include <vector>

#include <glad/gl.h>
// We don't need to include opengl <gl.h> because glfw does it already.
#include <GLFW/glfw3.h>

#include <vbo.hpp>
#include <ebo.hpp>

namespace Core
{
    class Mesh
    {
    private:
        /* data */
    public:
        Vbo vbo{};
        Ebo ebo{};
        std::vector<GLfloat> vertices;
        std::vector<GLuint> indices;

        Mesh(/* args */) = default;
        ~Mesh() = default;

        void gen();
        void bind();
    };

} // namespace Core
