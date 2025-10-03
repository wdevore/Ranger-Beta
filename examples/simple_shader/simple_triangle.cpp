#include <glad/gl.h>

#include "simple_triangle.hpp"
#include "misc.hpp"

namespace Game
{
    SimpleTriangle::SimpleTriangle(/* args */)
    {
    }

    SimpleTriangle::~SimpleTriangle()
    {
    }

    void SimpleTriangle::configure(gameEnvironmentShPtr environment)
    {
        shader.initialize(environment);
        Core::ErrorConditions buildStatus = shader.build();

        if (buildStatus != Core::ErrorConditions::None)
        {
            std::cerr << "Shader load, compiler and linker error" << std::endl;
            return;
        }

        program = shader.program();

        bind();

        vertexColorLocation = glGetUniformLocation(program, "ourColor");
    }

    void SimpleTriangle::setColor(const std::array<GLfloat, 4> &color)
    {
        glUniform4f(vertexColorLocation, color[0], color[1], color[2], color[3]);
        Game::checkGLError("glUniform4f");
    }

    void SimpleTriangle::render()
    {
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        // glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        checkGLError("glDrawElements");
    }

    void SimpleTriangle::dispose()
    {
        std::cout << "Disposing SimpleTriangle" << std::endl;
        glDeleteVertexArrays(1, &VAO);
        checkGLError("glDeleteVertexArrays");
        glDeleteBuffers(1, &VBO);
        checkGLError("glDeleteBuffers");

        glDeleteProgram(program);
        Game::checkGLError("glDeleteProgram");
    }

    void SimpleTriangle::bind()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);
        checkGLError("glBindVertexArray vao");

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        checkGLError("glBindBuffer vbo");
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GL_FLOAT), vertices.data(), GL_STATIC_DRAW);
        checkGLError("glBufferData vbo");

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        checkGLError("glBindBuffer bind EBO");
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
        checkGLError("glBufferData EBO");

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        checkGLError("glVertexAttribPointer");
        glEnableVertexAttribArray(0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);
        checkGLError("glBindVertexArray vao 2");
    }

    void SimpleTriangle::use()
    {
        glUseProgram(program);
        checkGLError("glUseProgram");

        glBindVertexArray(VAO);
        checkGLError("SimpleTriangle::use:glBindVertexArray");
    }
}