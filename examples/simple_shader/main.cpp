#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <memory>

#include <glad/gl.h>

// We don't need to include opengl <gl.h> because glfw does it already.
#include <GLFW/glfw3.h>

#include <vector2d.hpp>
#include <basic_shader.hpp>
#include <game_environment.hpp>
#include "misc.hpp"
#include "simple_triangle.hpp"

// Function prototypes

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(int, char **)
{
    std::cout << "Simple Shader!" << std::endl;
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    // if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    // {
    //     std::cout << "Failed to initialize GLAD" << std::endl;
    //     return -1;
    // }
    // Load OpenGL functions, gladLoadGL returns the loaded version, 0 on error.
    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0)
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        glfwTerminate();
        return -1;
    }

    const GLubyte *glVersion = glGetString(GL_VERSION);
    const GLubyte *glVendor = glGetString(GL_VENDOR);
    const GLubyte *glRenderer = glGetString(GL_RENDERER);

    if (glVersion && glVendor && glRenderer)
    {
        std::cout << "OpenGL Version: " << glVersion << std::endl;
        std::cout << "OpenGL Vendor: " << glVendor << std::endl;
        std::cout << "OpenGL Renderer: " << glRenderer << std::endl;
    }
    else
    {
        std::cerr << "Failed to retrieve OpenGL information." << std::endl;
    }

    // build and compile our shader program
    // ------------------------------------
    auto environment = std::make_shared<Game::GameEnvironment>();
    environment->initialize(SCR_WIDTH, SCR_HEIGHT);

    Game::SimpleTriangle triangle;
    triangle.configure(environment);

    Game::SimpleTriangle triangle2;
    triangle2.configure(environment);

    Game::SimpleTriangle triangle3;
    triangle3.configure(environment);

    Core::Matrix4 model{true};
    model.toIdentity();
    model.translate(125.0, 125.0, 0.0);
    model.scaleBy(100.0, 100.0, 1.0);

    Core::Matrix4 model2{true};
    model2.toIdentity();
    model2.translate(155.0, 155.0, 0.0);
    model2.scaleBy(100.0, 100.0, 1.0);

    Core::Matrix4 model3{true};
    model3.toIdentity();
    model3.translate(255.0, 255.0, 0.0);
    model3.scaleBy(100.0, 100.0, 1.0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        Game::checkGLError("glClearColor");
        glClear(GL_COLOR_BUFFER_BIT);
        Game::checkGLError("glClear");

        // be sure to activate the shader before any calls to glUniform
        // -------- Triangle 1 ----------------------
        triangle.use();

        // update shader uniform
        double timeValue = glfwGetTime();
        float greenValue = static_cast<float>(sin(timeValue) / 2.0 + 0.5);
        triangle.setColor({0.0f, greenValue, 0.0f, 1.0f});

        // render the triangle
        triangle.render(model);

        // -------- Triangle 2 ----------------------
        triangle2.use();
        triangle2.setColor({1.0f, 0.5, 0.0f, 1.0f});
        triangle2.render(model2);

        // -------- Triangle 3 ----------------------
        triangle3.use();
        triangle3.setColor({0.0f, 0.5, 1.0f, 1.0f});
        triangle3.render(model3);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    triangle.dispose();
    triangle2.dispose();
    triangle3.dispose();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}