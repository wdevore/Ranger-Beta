#include <iostream>
#include <iomanip>

#include <glad/gl.h>

// We don't need to include opengl <gl.h> because glfw does it already.
#include <GLFW/glfw3.h>

#include <vector2d.hpp>

// Window dimensions of GL canvas, not the OS window.
const GLuint DEVICE_WIDTH = 800, DEVICE_HEIGHT = 600;

// Function prototypes
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

int checkGLError(const std::string &funcName)
{
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "OpenGL Error in '" << funcName
                  << "': 0x" << std::hex << std::uppercase << std::setw(4) << std::setfill('0')
                  << error << std::endl;

        return -1;
    }

    return 0;
}

int main(int, char **)
{
    std::cout << "Hello, window!" << std::endl;
    Core::Vector2D v3{};

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow(DEVICE_WIDTH, DEVICE_HEIGHT, "LearnOpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);

    // Load OpenGL functions, gladLoadGL returns the loaded version, 0 on error.
    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0)
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Successfully loaded OpenGL
    std::cout << "Loaded OpenGL " << GLAD_VERSION_MAJOR(version) << "." << GLAD_VERSION_MINOR(version) << std::endl;

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

    // Define the viewport dimensions
    glViewport(0, 0, DEVICE_WIDTH, DEVICE_HEIGHT);
    checkGLError("glViewport");

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        checkGLError("glClearColor");
        glClear(GL_COLOR_BUFFER_BIT);
        checkGLError("glClear");

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    // Terminates GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
