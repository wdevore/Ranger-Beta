#include "primitive_ebo_test.hpp"

namespace Game
{
    PrimitiveEboTest::PrimitiveEboTest(/* args */)
    {
    }

    PrimitiveEboTest::~PrimitiveEboTest()
    {
    }

    Core::ErrorConditions PrimitiveEboTest::configure(Core::environmentShPtr environment)
    {
        std::string name = "PrimitiveEboTest";
        std::cout << name << "::configure" << std::endl;

        this->environment = environment;

        // Vertices for a single line segment
        std::vector<float> vertices = {
            -0.5f, -0.5f, 0.0f, // Vertex 0
            0.5f, 0.5f, 0.0f    // Vertex 1
        };

        // Indices for the line segment
        std::vector<unsigned int> indices = {
            0, 1 // Connects Vertex 0 to Vertex 1
        };

        // std::cout << name << "::_loadShaders" << std::endl;
        _loadShaders();

        Core::ErrorConditions configureUniStatus = configureUniforms();
        if (configureUniStatus != Core::ErrorConditions::None)
            return configureUniStatus;

        std::cout << name << "::bind" << std::endl;
        bind(vertices, indices);

        return Core::ErrorConditions::None;
    }

    Core::ErrorConditions PrimitiveEboTest::_loadShaders()
    {
        shader.initialize(environment);
        Core::ErrorConditions buildStatus = shader.build();

        return buildStatus;
    }

    Core::ErrorConditions PrimitiveEboTest::configureUniforms()
    {
        std::string name = "PrimitiveEboTest";
        std::cout << name << "::configureUniforms" << std::endl;
        int err{0};

        shader.use();

        GLuint program = shader.program();

        modelLoc = glGetUniformLocation(program, uniModel.c_str());
        err = Core::checkGLError(name + "::configureUniforms:glGetUniformLocation model");
        if (err < 0)
            return Core::ErrorConditions::GLFunctionError;
        if (modelLoc < 0)
        {
            lastError = "Couldn't find 'model' uniform variable";
            std::cout << name << ": " << lastError << std::endl;
            return Core::ErrorConditions::GLUniformVarNotFound;
        }
        std::cout << name << "::configureUniforms: modelLoc: " << modelLoc << std::endl;

        colorLoc = glGetUniformLocation(program, uniColor.c_str());
        err = Core::checkGLError(name + "::configureUniforms:glGetUniformLocation color");
        if (err < 0)
            return Core::ErrorConditions::GLFunctionError;
        if (colorLoc < 0)
        {
            lastError = "Couldn't find 'fragColor' uniform variable";
            std::cout << name << ": " << lastError << std::endl;
            return Core::ErrorConditions::GLUniformVarNotFound;
        }
        std::cout << name << "::configureUniforms: colorLoc: " << colorLoc << std::endl;

        // One time configuration of projection and view matrix
        GLint projLoc = glGetUniformLocation(program, uniProjection.c_str());
        err = Core::checkGLError(name + "::configureUniforms:glGetUniformLocation proj");
        if (err < 0)
            return Core::ErrorConditions::GLFunctionError;
        if (projLoc < 0)
        {
            lastError = "Couldn't find 'projection' uniform variable";
            std::cout << name << ": " << lastError << std::endl;
            return Core::ErrorConditions::GLUniformVarNotFound;
        }
        std::cout << name << "::configureUniforms: projLoc: " << projLoc << std::endl;

        GLint viewLoc = glGetUniformLocation(program, uniView.c_str());
        err = Core::checkGLError(name + "::configureUniforms:glGetUniformLocation view");
        if (err < 0)
            return Core::ErrorConditions::GLFunctionError;
        if (viewLoc < 0)
        {
            lastError = "Couldn't find 'view' uniform variable";
            std::cout << name << ": " << lastError << std::endl;
            return Core::ErrorConditions::GLUniformVarNotFound;
        }
        std::cout << name << "::configureUniforms: viewLoc: " << viewLoc << std::endl;

        Core::Matrix4 pm = projection.getMatrix();
        glUniformMatrix4fv(projLoc, GLUniformMatrixCount, GLUniformMatrixTransposed, pm.data());
        err = Core::checkGLError(name + "::configureUniforms:glUniformMatrix4fv(1)");
        if (err < 0)
            return Core::ErrorConditions::GLFunctionError;

        glUniformMatrix4fv(viewLoc, GLUniformMatrixCount, GLUniformMatrixTransposed, environment->camera.viewspace.data());
        err = Core::checkGLError(name + "::configureUniforms:glUniformMatrix4fv(2)");
        if (err < 0)
            return Core::ErrorConditions::GLFunctionError;

        return Core::ErrorConditions::None;
    }

    void PrimitiveEboTest::bind(const std::vector<float> &vertices,
                                const std::vector<unsigned int> &indices)
    {
        glGenVertexArrays(1, &VAO);
        glCheck(1);
        // Bind the VAO first
        glBindVertexArray(VAO);
        glCheck(2);

        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // Bind the VBO and upload vertex data
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glCheck(3);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GL_FLOAT), vertices.data(), GL_STATIC_DRAW);
        glCheck(4);

        // Bind the EBO and upload index data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glCheck(5);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
        glCheck(6);

        // Set vertex attribute pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid *)(0));
        glCheck(7);

        glEnableVertexAttribArray(Core::VertexAttribIndex);
        glCheck(70);

        // Unbind VBO, and VAO
        glBindBuffer(GL_ARRAY_BUFFER, Core::UnBindID);
        glCheck(71);

        glBindVertexArray(Core::UnBindID);
        // NOTE: Don't unbind the EBO while the VAO is active,
        // or the EBO's state will be lost.
        // The EBO is managed by the VAO's state.
    }

    void PrimitiveEboTest::setColor(const std::array<GLfloat, 4> &color)
    {
        glUniform4fv(colorLoc, Uniform4vColorCompCount, color.data());
        glCheck(8);
    }

    void PrimitiveEboTest::render(const Core::Matrix4 &model)
    {
        glUniformMatrix4fv(modelLoc, GLUniformMatrixCount, GLUniformMatrixTransposed, model.data());
        glCheck(9);

        // Bind the VAO
        // glBindVertexArray(VAO);
        // glCheck(19);

        // Draw the line using the EBO
        glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, (void *)0);
        glCheck(10);

        // Unbind the VAO
        // glBindVertexArray(0);
        // glCheck(20);
    }

    void PrimitiveEboTest::use()
    {
        glBindVertexArray(VAO);
        glCheck(22);

        shader.use();
        glCheck(21);
    }

    void PrimitiveEboTest::unUse()
    {
        glBindVertexArray(Core::UnBindID);
    }

    void PrimitiveEboTest::glCheck(int markId)
    {
        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            // Handle the error here
            std::cerr << "########## (" << std::dec << markId << ") OpenGL Error: " << error << std::endl;
        }
    }

    void PrimitiveEboTest::dispose()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &EBO);
        glDeleteBuffers(1, &VBO);

        GLuint program = shader.program();
        glDeleteProgram(program);
    }
} // namespace Game
