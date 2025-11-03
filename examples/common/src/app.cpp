
#include <iostream>
#include <sstream>
#include <iomanip>

#include "app.hpp"
#include "global_data.hpp"

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
        preSetup();

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

    int App::configure()
    {
        std::cout << "App::configure" << std::endl;

        setCallbacks();

        loadGlad();

        showOpenGLInfo();

        setViewport();

        return 0;
    }

    int App::setCallbacks()
    {
        // Set the required callback functions
        glfwSetKeyCallback(window_, key_callback);
        glfwSetCursorPosCallback(window_, cursor_position_callback);
        glfwSetMouseButtonCallback(window_, mouse_button_callback);
        glfwSetScrollCallback(window_, scroll_callback);
        glfwSetCursorEnterCallback(window_, cursor_enter_callback);

        return 0;
    }

    int App::loadGlad()
    {
        // Load OpenGL functions, gladLoadGL returns the loaded version, 0 on error.
        int version = gladLoadGL(glfwGetProcAddress);
        if (version == 0)
        {
            glfwTerminate();
            return -1;
        }

        // Successfully loaded OpenGL
        std::cout << "App::configure Loaded GLAD version:"
                  << GLAD_VERSION_MAJOR(version) << "." << GLAD_VERSION_MINOR(version) << std::endl
                  << "--------------------------"
                  << " GL calls can now be made "
                  << "--------------------------"
                  << std::endl;

        return 0;
    }

    void App::showOpenGLInfo()
    {
        // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
        // ---- Anything GL wise can be called after this point. -----
        // -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

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
    }

    int App::setViewport()
    {
        glViewport(0, 0, width, height);
        int err = Core::checkGLError("App::createWindow:glViewport");
        if (err < 0)
        {
            glfwTerminate();
            return -1;
        }

        return 0;
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
        // Store the 'this' pointer of the App instance in the window's user pointer
        glfwSetWindowUserPointer(window_, this);

        glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);

        // Set initial viewport size
        int width_, height_;
        glfwGetFramebufferSize(window_, &width_, &height_);
        std::cout << "App::createWindow Window size: " << width_ << "x" << height_ << std::endl;

        return true;
    }

    void App::dispose()
    {
        std::cout << "App::dispose" << std::endl;

        pressedKeys.clear();

        // optional: de-allocate all resources once they've outlived their purpose:
        // ------------------------------------------------------------------------
        // Call any deconstructions on classes that implemented deconstruct().
        int deconstructStatus = deconstruct();

        // Detach callbacks
        glfwSetKeyCallback(window_, nullptr);
        glfwSetCursorPosCallback(window_, nullptr);
        glfwSetMouseButtonCallback(window_, nullptr);
        glfwSetScrollCallback(window_, nullptr);
        glfwSetCursorEnterCallback(window_, nullptr);

        // Terminates GLFW, clearing any resources allocated by GLFW.
        glfwTerminate();
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
                std::stringstream ss;
                // Set formatting flags:
                // 1. std::fixed: Forces fixed-point notation (ensures a decimal point is always shown).
                // 2. std::setprecision(2): Sets the number of digits *after the decimal point* to exactly 2.
                ss << std::fixed << std::setprecision(2);

                ss << "FPS: " << currentFPS;
                currentFPSText = ss.str();
                // std::cout << currentFPSText << std::endl;
            }

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window_);

            previousFrameTime = currentFrameTime;

            // Optional: Limit frame rate (if needed)
            // std::this_thread::sleep_for(targetFrameDuration - deltaTime);
        }

        return 0;
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

    void App::setupSmoothing()
    {
        glEnable(GL_BLEND);
        Core::checkGLError("App::glEnable(GL_BLEND)");

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        Core::checkGLError("App::glBlendFunc");
    }

    // --------------------------------------------------------------------
    // IO Event callbacks
    // --------------------------------------------------------------------
    Core::IOEvent::Modifier App::modifierToEnum(int modifier)
    {
        Core::IOEvent::Modifier modi{Core::IOEvent::Modifier::Unknown};
        if (modifier & GLFW_MOD_SHIFT)
        {
            modi = Core::IOEvent::Modifier::Shift;
        }
        if (modifier & GLFW_MOD_CONTROL)
        {
            modi = Core::IOEvent::Modifier::Control;
        }
        if (modifier & GLFW_MOD_ALT)
        {
            modi = Core::IOEvent::Modifier::Alt;
        }
        if (modifier & GLFW_MOD_SUPER)
        {
            modi = Core::IOEvent::Modifier::Super;
        }

        return modi;
    }

    Core::IOEvent::Action App::actionToEnum(int action)
    {
        Core::IOEvent::Action act{Core::IOEvent::Action::Unknown};
        if (action == GLFW_PRESS)
        {
            act = Core::IOEvent::Action::Press;
        }
        else if (action == GLFW_RELEASE)
        {
            act = Core::IOEvent::Action::Release;
        }
        else if (action == GLFW_REPEAT)
        {
            act = Core::IOEvent::Action::Repeat;
        }

        return act;
    }

    Core::IOEvent::Button App::buttonToEnum(int button)
    {
        Core::IOEvent::Button buto{Core::IOEvent::Button::Unknown};
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            buto = Core::IOEvent::Button::Left;
        }
        else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            buto = Core::IOEvent::Button::Right;
        }
        else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
        {
            buto = Core::IOEvent::Button::Middle;
        };

        return buto;
    }

    // Is called whenever a key is pressed/released via GLFW
    void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
            return;
        }

        Game::App *app = static_cast<Game::App *>(glfwGetWindowUserPointer(window));
        if (app)
        {
            if (action == GLFW_PRESS || action == GLFW_REPEAT)
            {
                app->registerKeyPressed(key);
            }
            else if (action == GLFW_RELEASE)
            {
                app->registerKeyReleased(key);
            }

            app->ioEvent.setKeyboardEvent(key, scancode, app->actionToEnum(action), app->modifierToEnum(mode));
            app->processIOEvent();
        }
    }

    // Callback function for cursor position
    // Mouse coordinate system has the origin at the top-left.
    void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
    {
        // std::cout << "Mouse moved to (" << xpos << ", " << ypos << ")" << std::endl;
        // You can store the mouse position in your application here
        Game::App *app = static_cast<Game::App *>(glfwGetWindowUserPointer(window));
        if (app)
        {
            // app->ioEvent.reset();
            app->ioEvent.setMouseMoveEvent(xpos, ypos);
            app->processIOEvent();
        }
    }

    // Callback function for mouse button events
    void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
    {
        // std::cout << "Mouse button (" << button << ")" << std::endl;
        Game::App *app = static_cast<Game::App *>(glfwGetWindowUserPointer(window));
        if (app)
        {
            // app->ioEvent.reset();
            app->ioEvent.setMouseButtonEvent(app->buttonToEnum(button), app->actionToEnum(action), app->modifierToEnum(mods));
            app->processIOEvent();
        }
    }

    // Callback function for mouse wheel scrolling
    void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
    {
        // std::cout << "Mouse wheel scrolled (x offset: " << xoffset << ", y offset: " << yoffset << ")" << std::endl;
        // Typically, yoffset represents vertical scrolling (positive for up, negative for down)
        // xoffset represents horizontal scrolling (if the mouse supports it)
        Game::App *app = static_cast<Game::App *>(glfwGetWindowUserPointer(window));
        if (app)
        {
            app->ioEvent.setMouseWheelEvent(xoffset, yoffset);
            app->processIOEvent();
        }
    }

    // Callback function for cursor entering or leaving the window
    void cursor_enter_callback(GLFWwindow *window, int entered)
    {
        Game::App *app = static_cast<Game::App *>(glfwGetWindowUserPointer(window));
        if (app)
        {
            app->ioEvent.setMouseEnterExitEvent(static_cast<bool>(entered));
            app->processIOEvent();
        }
    }

} // namespace Game
