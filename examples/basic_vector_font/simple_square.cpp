#include "simple_square.hpp"
namespace Game
{
    SimpleSquare::SimpleSquare()
    {
    }

    SimpleSquare::~SimpleSquare()
    {
    }

    void SimpleSquare::configure(Core::environmentShPtr environment)
    {
        this->environment = environment;
        loadCompileLinkShaders();
        bind();
    }

    void SimpleSquare::loadCompileLinkShaders()
    {
        shader.initialize(environment);
        Core::ErrorConditions buildStatus = shader.build();

        if (buildStatus != Core::ErrorConditions::None)
        {
            std::cerr << "Shader load, compiler and linker error" << std::endl;
            return;
        }
    }

    void SimpleSquare::bind()
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

    void SimpleSquare::render(const Core::Matrix4 &model)
    {
        glBindVertexArray(VAO);
        Core::checkGLError(name + "::render bind");

        glUseProgram(shader.program());
        Core::checkGLError(name + "::render use program");

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        Core::checkGLError(name + "::render draw elements");
    }

} // namespace Game
