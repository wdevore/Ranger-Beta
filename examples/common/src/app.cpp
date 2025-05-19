
#include <iostream>

#include "app.hpp"
#include <constants.hpp>

namespace Game
{

    // System Events
    void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
    void framebuffer_size_callback(GLFWwindow *window, int width, int height);

    // IO Events
    void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
    void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
    void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
    void cursor_enter_callback(GLFWwindow *window, int entered);

    /// @brief This is the first method called by main.cpp
    /// @return
    int App::initialize()
    {
        std::cout << "App::initialize" << std::endl;

        int initialized = glfwInit();
        if (initialized)
        {
            initialized = setup();
        }

        previousFrameTime = Clock::now();
        frameCounter = 0;
        currentFPS = 0.0;

        return initialized;
    }

    bool App::createWindow(const std::string &title)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        // Create a GLFWwindow object that we can use for GLFW's functions
        window_ = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        if (window_ == NULL)
        {
            glfwTerminate();
            return false;
        }
        glfwMakeContextCurrent(window_);
        glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);

        // Set initial viewport size
        int width_, height_;
        glfwGetFramebufferSize(window_, &width_, &height_);
        std::cout << "App::createWindow Window size: " << width_ << "x" << height_ << std::endl;

        return true;
    }

    int App::configure()
    {
        std::cout << "App::configure" << std::endl;

        // Set the required callback functions
        glfwSetKeyCallback(window_, key_callback);
        glfwSetCursorPosCallback(window_, cursor_position_callback);
        glfwSetMouseButtonCallback(window_, mouse_button_callback);
        glfwSetScrollCallback(window_, scroll_callback);
        glfwSetCursorEnterCallback(window_, cursor_enter_callback);

        // Load OpenGL functions, gladLoadGL returns the loaded version, 0 on error.
        int version = gladLoadGL(glfwGetProcAddress);
        if (version == 0)
        {
            glfwTerminate();
            return -1;
        }

        // Successfully loaded OpenGL
        std::cout << "App::configure Loaded OpenGL "
                  << GLAD_VERSION_MAJOR(version) << "." << GLAD_VERSION_MINOR(version) << std::endl
                  << "--------------------------"
                  << " GL calls can now be made "
                  << "--------------------------"
                  << std::endl;

        // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
        // ---- Anything GL wise can be called after this point. -----
        // -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
        glViewport(0, 0, width, height);
        int err = Core::checkGLError("App::createWindow:glViewport");
        if (err < 0)
        {
            glfwTerminate();
            return -1;
        }

        return 0;
    }

    int App::run()
    {
        int configured = verifyConfigured();
        if (configured < 0)
            return -1;

        std::cout << "App::run Entering game loop" << std::endl;

        // Game loop
        while (!glfwWindowShouldClose(window_))
        {
            const TimePoint currentFrameTime = Clock::now();
            const Duration deltaTime = currentFrameTime - previousFrameTime;
            const double dt = deltaTime.count(); // Get delta time in seconds

            // Check if any events have been activated (key pressed, mouse moved
            // etc.) and call corresponding response functions
            glfwPollEvents();

            // TODO add logic to determine when to call update. See
            // Gemini response.
            // std::cout << "dt: " << dt << std::endl;
            update(dt * Core::TO_MILLISECONDS);

            render();
            frameCounter++;

            const TimePoint currentTime = Clock::now();
            const Duration timeSinceLastFPSUpdate = currentTime - lastFPSTime;

            // Update FPS roughly every second
            if (timeSinceLastFPSUpdate.count() >= 1.0)
            {
                currentFPS = static_cast<double>(frameCounter) / timeSinceLastFPSUpdate.count();
                frameCounter = 0;
                lastFPSTime = currentTime;

                // Now 'currentFPS' holds the frames per second
                // std::cout << "currentFPS: " << currentFPS << std::endl;
            }

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window_);

            previousFrameTime = currentFrameTime;

            // Optional: Limit frame rate (if needed)
            // std::this_thread::sleep_for(targetFrameDuration - deltaTime);
        }

        // optional: de-allocate all resources once they've outlived their purpose:
        // ------------------------------------------------------------------------
        int deconstructStatus = deconstruct();

        // Detach callbacks
        glfwSetKeyCallback(window_, nullptr);
        glfwSetCursorPosCallback(window_, nullptr);
        glfwSetMouseButtonCallback(window_, nullptr);
        glfwSetScrollCallback(window_, nullptr);
        glfwSetCursorEnterCallback(window_, nullptr);

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
        std::cout << "GL View dimensions: " << width << "x" << height << std::endl;

        // make sure the viewport matches the new window dimensions; note that width and
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
    }

    // Callback function for cursor position
    // Mouse coordinate system has the origin at the top-left.
    void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
    {
        std::cout << "Mouse moved to (" << xpos << ", " << ypos << ")" << std::endl;
        // You can store the mouse position in your application here
    }

    // Callback function for mouse button events
    void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
    {
        if (action == GLFW_PRESS)
        {
            if (button == GLFW_MOUSE_BUTTON_LEFT)
            {
                std::cout << "Left mouse button pressed" << std::endl;
            }
            else if (button == GLFW_MOUSE_BUTTON_RIGHT)
            {
                std::cout << "Right mouse button pressed" << std::endl;
            }
            else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
            {
                std::cout << "Middle mouse button pressed" << std::endl;
            }
            else
            {
                std::cout << "Other mouse button pressed: " << button << std::endl;
            }
        }
        else if (action == GLFW_RELEASE)
        {
            if (button == GLFW_MOUSE_BUTTON_LEFT)
            {
                std::cout << "Left mouse button released" << std::endl;
            }
            else if (button == GLFW_MOUSE_BUTTON_RIGHT)
            {
                std::cout << "Right mouse button released" << std::endl;
            }
            else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
            {
                std::cout << "Middle mouse button released" << std::endl;
            }
            else
            {
                std::cout << "Other mouse button released: " << button << std::endl;
            }
        }
        // 'mods' indicates which modifier keys (Shift, Ctrl, Alt, Super) were held down
    }

    // Callback function for mouse wheel scrolling
    void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
    {
        std::cout << "Mouse wheel scrolled (x offset: " << xoffset << ", y offset: " << yoffset << ")" << std::endl;
        // Typically, yoffset represents vertical scrolling (positive for up, negative for down)
        // xoffset represents horizontal scrolling (if the mouse supports it)
    }

    // Callback function for cursor entering or leaving the window
    void cursor_enter_callback(GLFWwindow *window, int entered)
    {
        if (entered)
        {
            std::cout << "Mouse cursor entered the window" << std::endl;
            // The mouse cursor is now inside the client area of the window
        }
        else
        {
            std::cout << "Mouse cursor left the window" << std::endl;
            // The mouse cursor is no longer inside the client area of the window
        }
    }
} // namespace Game
