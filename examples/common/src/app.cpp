
#include <iostream>

#include "app.hpp"

namespace Game
{

    // Function prototypes
    void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

    App::~App()
    {
    }

    int App::initialize()
    {
        int initialized = glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        return initialized;
    }

    bool App::createWindow(const std::string &title)
    {
        // Create a GLFWwindow object that we can use for GLFW's functions
        window_ = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        glfwMakeContextCurrent(window_);
        if (window_ == NULL)
        {
            glfwTerminate();
            return false;
        }

        return true;
    }

    int App::configure()
    {
        // Set the required callback functions
        glfwSetKeyCallback(window_, key_callback);

        // Load OpenGL functions, gladLoadGL returns the loaded version, 0 on error.
        int version = gladLoadGL(glfwGetProcAddress);
        if (version == 0)
        {
            glfwTerminate();
            return -1;
        }

        // Successfully loaded OpenGL
        std::cout << "Loaded OpenGL " << GLAD_VERSION_MAJOR(version) << "." << GLAD_VERSION_MINOR(version) << std::endl;

        // Define the viewport dimensions
        glViewport(0, 0, width, height);

        return 0;
    }

    int App::run()
    {
        // Game loop
        while (!glfwWindowShouldClose(window_))
        {
            // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
            glfwPollEvents();

            // Render
            // Clear the colorbuffer
            glClearColor(bgClearColor.r, bgClearColor.g, bgClearColor.b, bgClearColor.a);
            glClear(GL_COLOR_BUFFER_BIT);

            // Swap the screen buffers
            glfwSwapBuffers(window_);
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
} // namespace Game
