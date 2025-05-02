#include <iostream>
#include <memory>

#include <gameapp.hpp>
#include <basic_scene_node.hpp>

namespace Game
{
    GameApp::~GameApp()
    {
    }

    int GameApp::verifyConfigured()
    {
        if (nodeMan.nodes.size() < 1)
        {
            std::cout << "App is not configured correctly. There must be at least 1 node." << std::endl;
            return -1;
        }

        return 0;
    }

    // Called by App::configure
    int GameApp::compile()
    {
        std::cout << "Compiling shaders" << std::endl;

        // build and compile our shader program
        // ------------------------------------
        // vertex shader
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        // check for shader compile errors
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
            return -1;
        }

        // fragment shader
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        // check for shader compile errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
            return -1;
        }

        // link shaders
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        // check for linking errors
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                      << infoLog << std::endl;
            return -1;
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return 1; // success
    }

    // Called by App::configure
    int GameApp::build()
    {
        std::cout << "Building Atlas" << std::endl;

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float vertices[] = {
            -0.5f, -0.5f, 0.0f, // left
            0.5f, -0.5f, 0.0f,  // right
            0.0f, 0.5f, 0.0f    // top
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);

        return 1; // Success
    }

    int GameApp::deconstruct()
    {
        std::cout << "Deconstructing" << std::endl;

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(shaderProgram);

        return 1; // success
    }

    int GameApp::setup()
    {
        // Load any assets, for example vector font.

        // Initialize any objects
        nodeMan.initialize();

        return 1;
    }

    int GameApp::construct()
    {
        std::cout << "Constructing game" << std::endl;

        // Create a single scene Node to hold a square.
        basicScene = std::make_shared<BasicScene>("BasicScene");
        nodeMan.push(basicScene);

        return 1;
    }

    void GameApp::render()
    {
        // std::cout << "rendering" << std::endl;

        // Render
        nodeMan.visit(0.0, width, height);

        // Clear the colorbuffer
        glClearColor(bgClearColor.r, bgClearColor.g, bgClearColor.b, bgClearColor.a);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0); // no need to unbind it every time
    }

    int GameApp::update(double dt)
    {

        nodeMan.update(dt);

        return 1; // Successful update
    }

} // namespace Game
