#include <glad/gl.h>

#include "global_data.hpp"
#include "simple_ubo.hpp"
#include "misc.hpp"

extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;

namespace Game
{
    SimpleUbo::SimpleUbo(/* args */)
    {
    }

    SimpleUbo::~SimpleUbo()
    {
    }

    void SimpleUbo::configure()
    {
        // this->environment = environment;

        shader.initialize();
        Core::ErrorConditions buildStatus = shader.build();

        if (buildStatus != Core::ErrorConditions::None)
        {
            std::cerr << "Shader load, compiler and linker error" << std::endl;
            return;
        }

        program = shader.program();
        std::cout << "Program: " << program << std::endl;

        bind();

        // Uniform buffer objects:
        // When using more than one shader we continuously have to set
        // uniform variables where most of them are exactly the same for each shader.
        // Because of this:
        // OpenGL gives us a tool called uniform buffer objects that allow us to
        // declare a set of global uniform variables that remain the same over
        // any number of shader programs. When using uniform buffer objects we
        // set the relevant uniforms only once in fixed GPU memory.
        // We do still have to manually set the uniforms that are unique per shader.
        // Creating and configuring a uniform buffer object requires a bit of work though.

        fetchUniforms();

        // -----------------------------------------------------------
        // Origin at top-left
        // This aligns with the device where the origin is in the upper-left.
        // This mean +Y is downward
        // -----------------------------------------------------------
        const float near{-1.0};
        const float far{1.0};
        const float w = static_cast<float>(SCR_WIDTH);
        const float h = static_cast<float>(SCR_HEIGHT);

        projection.setProjection(h, 0.0, // bottom-left
                                 0.0, w, // top-right
                                 near, far);
    }

    void SimpleUbo::fetchUniforms()
    {
        int err{0};
        std::string lastError{};

        modelLoc = fetchUniformVar(program, uniModel);
        if (modelLoc < 0)
            return;

        colorLoc = fetchUniformVar(program, uniColor);
        if (colorLoc < 0)
            return;

        // viewLoc = fetchUniformVar(program, uniView);
        // if (viewLoc < 0)
        //     return;

        // projectLoc = fetchUniformVar(program, uniProjection);
        // if (projectLoc < 0)
        //     return;
    }

    GLint SimpleUbo::fetchUniformVar(GLuint program, const std::string &name)
    {
        GLint location = glGetUniformLocation(program, name.c_str());
        int err = Core::checkGLError("::fetchUniforms:glGetUniformLocation model");
        if (err < 0)
        {
            return -1;
        }
        if (location < 0)
        {
            std::cout << "Fetch Uniform Error: " << "Couldn't find '" + name + "' uniform variable" << std::endl;
            return -1;
        }
        std::cout << "::fetchUniforms: location for '" << name << "': " << location << std::endl;
        return location;
    }

    void SimpleUbo::setColor(const std::array<GLfloat, 4> &color)
    {
        glUniform4f(colorLoc, color[0], color[1], color[2], color[3]);
        Game::checkGLError("glUniform4f");
    }

    int SimpleUbo::begin()
    {
        // Core::Matrix4 pm = projection.getMatrix();
        // glUniformMatrix4fv(projectLoc, GLUniformMatrixCount, GLUniformMatrixTransposed, pm.data());
        // int err = Core::checkGLError("begin::configureUniforms:glUniformMatrix4fv project");
        // if (err < 0)
        //     return -1;

        // glUniformMatrix4fv(viewLoc, GLUniformMatrixCount, GLUniformMatrixTransposed, environment->camera.viewspace.data());
        // err = Core::checkGLError("begin::configureUniforms:glUniformMatrix4fv view");
        // if (err < 0)
        //     return -1;

        return 0;
    }

    void SimpleUbo::render(const Core::Matrix4 &model)
    {
        begin();
        glUniformMatrix4fv(modelLoc, GLUniformMatrixCount, GLUniformMatrixTransposed, model.data());
        Core::checkGLError("begin::configureUniforms:glUniformMatrix4fv model");

        // glDrawArrays(GL_TRIANGLES, 0, 3);
        // glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        Core::checkGLError("glDrawElements");
    }

    void SimpleUbo::dispose()
    {
        std::cout << "Disposing SimpleUbo" << std::endl;
        glDeleteVertexArrays(1, &VAO);
        Core::checkGLError("glDeleteVertexArrays VAO");
        glDeleteBuffers(1, &VBO);
        Core::checkGLError("glDeleteBuffers VBO");
        glDeleteBuffers(1, &EBO);
        Core::checkGLError("glDeleteBuffers EBO");

        glDeleteProgram(program);
        Core::checkGLError("glDeleteProgram");
    }

    void SimpleUbo::bind()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        std::cout << "VAO: " << VAO << ", VBO: " << VBO << ", EBO: " << EBO << std::endl;

        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);
        Core::checkGLError("glBindVertexArray vao");

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        Core::checkGLError("glBindBuffer vbo");
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GL_FLOAT), vertices.data(), GL_STATIC_DRAW);
        Core::checkGLError("glBufferData vbo");

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        Core::checkGLError("glBindBuffer bind EBO");
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
        Core::checkGLError("glBufferData EBO");

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        Core::checkGLError("glVertexAttribPointer");
        glEnableVertexAttribArray(0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);
        Core::checkGLError("glBindVertexArray vao 2");
    }

    void SimpleUbo::use()
    {
        glUseProgram(program);
        Core::checkGLError("glUseProgram");

        glBindVertexArray(VAO);
        Core::checkGLError("SimpleUbo::use:glBindVertexArray VAO");
    }
}