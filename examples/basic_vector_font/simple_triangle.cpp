#include "simple_triangle.hpp"

namespace Game
{
    SimpleTriangle::SimpleTriangle()
    {
    }

    SimpleTriangle::~SimpleTriangle()
    {
    }

    void SimpleTriangle::configure()
    {
        name = "SimpleTriangle";
        // shader.specifyShaderFiles("mono_vertex.glsl", "mono_fragment.glsl");
        shader.specifyShaderFiles("mono_vert_ubo.glsl", "mono_fragment.glsl");

        generator.generateUnitTriangle(Core::ShapeControls::Centered, Core::ShapeControls::Filled);
        vertices = generator.shape.vertices;
        indices = generator.shape.indices;

        loadCompileLinkShaders();
        configureUniforms();
        setProjectionView();
        bind();
    }

    void SimpleTriangle::loadCompileLinkShaders()
    {
        shader.initialize();
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

        // bind the Vertex Array Object first, then bind and set vertex buffer(s),
        // and then configure vertex attributes(s).
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

        // note that this is allowed, the call to glVertexAttribPointer registered
        // VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, Core::UnBindID);
        Core::checkGLError(name + "::glBindBuffer");

        // remember: do NOT unbind the EBO while a VAO is active as the bound
        // element buffer object IS stored in the VAO; keep the EBO bound.
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Core::UnBindID);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally
        // modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally
        // don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(Core::UnBindID);
        Core::checkGLError(name + "::glBindVertexArray");
    }

    Core::ErrorConditions SimpleTriangle::configureUniforms()
    {
        std::cout << name << "::configureUniforms" << std::endl;
        int err{0};

        shader.use();

        GLuint program = shader.program();

        modelLoc = fetchUniformVar(program, uniModel);
        if (modelLoc < 0)
            return Core::ErrorConditions::GLUniformVarNotFound;

        colorLoc = fetchUniformVar(program, uniColor);
        if (colorLoc < 0)
            return Core::ErrorConditions::GLUniformVarNotFound;

        return Core::ErrorConditions::None;
    }

    void SimpleTriangle::setColor(const std::array<GLfloat, 4> &color)
    {
        glUniform4fv(colorLoc, Uniform4vColorCompCount, color.data());
        Core::checkGLError(name + "::setColor");
    }

    GLint SimpleTriangle::fetchUniformVar(GLuint program, const std::string &programName)
    {
        GLint location = glGetUniformLocation(program, programName.c_str());
        int err = Core::checkGLError("::fetchUniforms:glGetUniformLocation model");
        if (err < 0)
        {
            return -1;
        }
        if (location < 0)
        {
            std::cout << "Fetch Uniform Error: " << "Couldn't find '" + programName + "' uniform variable" << std::endl;
            return -1;
        }
        std::cout << name << "::fetchUniforms: location for '" << programName << "': " << location << std::endl;
        return location;
    }

    void SimpleTriangle::setProjectionView()
    {
        const float near{-1.0};
        const float far{1.0};
        const float w = static_cast<float>(Core::deviceWidth);
        const float h = static_cast<float>(Core::deviceHeight);

        projection.setProjection(h, 0.0, // bottom-left
                                 0.0, w, // top-right
                                 near, far);
        Core::projection = projection.getMatrix();
    }

    void SimpleTriangle::begin()
    {
        glBindVertexArray(VAO);
        Core::checkGLError(name + "::begin");
    }

    void SimpleTriangle::render(const Core::Matrix4 &model)
    {
        glUniformMatrix4fv(modelLoc, GLUniformMatrixCount, GLUniformMatrixTransposed, model.data());
        Core::checkGLError("SimpleTriangle::render glUniformMatrix4fv model");

        glDrawElements(generator.shape.primitiveMode, indices.size(), GL_UNSIGNED_INT, 0);
        Core::checkGLError(name + "::render draw elements");
    }

    void SimpleTriangle::use()
    {
        shader.use();
        Core::checkGLError(name + "::use");
    }

    void SimpleTriangle::unUse()
    {
    }

    void SimpleTriangle::dispose()
    {
        std::cout << name << "::dispose" << std::endl;

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &EBO);
        glDeleteBuffers(1, &VBO);

        GLuint program = shader.program();
        glDeleteProgram(program);
    }

} // namespace Game
