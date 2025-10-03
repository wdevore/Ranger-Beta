#include "simple_triangle.hpp"
namespace Game
{
    SimpleTriangle::SimpleTriangle()
    {
    }

    SimpleTriangle::~SimpleTriangle()
    {
    }

    void SimpleTriangle::configure(Core::environmentShPtr environment)
    {
        this->environment = environment;
        loadCompileLinkShaders();
        configureUniforms();
        bind();
    }

    void SimpleTriangle::loadCompileLinkShaders()
    {
        shader.initialize(environment);
        Core::ErrorConditions buildStatus = shader.build();

        if (buildStatus != Core::ErrorConditions::None)
        {
            std::cerr << "Shader load, compiler and linker error" << std::endl;
            return;
        }
    }

    void SimpleTriangle::bind()
    {
        glGenVertexArrays(1, &VAO);

        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        Core::checkGLError(name + "::glBindBuffer bind VBO");
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GL_FLOAT), vertices.data(), GL_STATIC_DRAW);
        Core::checkGLError(name + "::glBufferData VBO");

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        Core::checkGLError(name + "::glBindBuffer bind EBO");
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
        Core::checkGLError(name + "::glBufferData EBO");

        // Specify the format of the data
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        Core::checkGLError(name + "::glVertexAttribPointer");
        glEnableVertexAttribArray(0);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        Core::checkGLError(name + "::glBindBuffer");

        // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);
        Core::checkGLError(name + "::glBindVertexArray");
    }

    Core::ErrorConditions SimpleTriangle::configureUniforms()
    {
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

    void SimpleTriangle::setColor(const std::array<GLfloat, 4> &color)
    {
        glUniform4fv(colorLoc, Uniform4vColorCompCount, color.data());
        Core::checkGLError(name + "::setColor");
    }

    void SimpleTriangle::begin()
    {
        glBindVertexArray(VAO);
        Core::checkGLError(name + "::begin");
    }

    void SimpleTriangle::render(const Core::Matrix4 &model)
    {

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        Core::checkGLError(name + "::render");
    }

    void SimpleTriangle::use()
    {
        shader.use();
        Core::checkGLError(name + "::use");
    }

    void SimpleTriangle::unUse()
    {
    }

} // namespace Game
