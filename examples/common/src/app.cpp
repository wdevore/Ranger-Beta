
#include <iostream>

#include "app.hpp"

namespace Game
{

    // Function prototypes
    void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
    void framebuffer_size_callback(GLFWwindow *window, int width, int height);

    App::~App()
    {
    }

    int App::initialize()
    {
        int initialized = glfwInit();
        if (initialized)
        {
            initialized = setup();
        }
        return initialized;
    }

    bool App::createWindow(const std::string &title)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        // Create a GLFWwindow object that we can use for GLFW's functions
        window_ = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        if (window_ == NULL)
        {
            glfwTerminate();
            return false;
        }
        glfwMakeContextCurrent(window_);
        glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);

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

        int compileStatus = compile();
        if (compileStatus < 0)
        {
            return -1;
        }

        // Define the viewport dimensions
        // glViewport(0, 0, width, height);

        return 0;
    }

    int App::run()
    {
        int configured = verifyConfigured();
        if (configured < 0)
        {
            return -1;
        }

        // Game loop
        while (!glfwWindowShouldClose(window_))
        {
            // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
            glfwPollEvents();

            // TODO add logic to determine when to call update.
            update(0.0);

            render();

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window_);
        }

        // optional: de-allocate all resources once they've outlived their purpose:
        // ------------------------------------------------------------------------
        int deconstructStatus = deconstruct();

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

    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    // ---------------------------------------------------------------------------------------------
    void framebuffer_size_callback(GLFWwindow *window, int width, int height)
    {
        // make sure the viewport matches the new window dimensions; note that width and
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
    }
} // namespace Game
