#pragma once
#include <chrono>
#include <string>
#include <memory>
#include <unordered_set>

#include <glad/gl.h>
// We don't need to include opengl <gl.h> because glfw does it already.
#include <GLFW/glfw3.h>

#include <color4.hpp>
#include <io_event.hpp>

namespace Game
{
    /// @brief Abstract class
    class App
    {
    protected:
        /* data */
        GLFWwindow *window_;

        // ------------ Timing ------------------
        using Clock = std::chrono::high_resolution_clock;
        using TimePoint = Clock::time_point;
        using Duration = std::chrono::duration<double>; // Delta time in seconds

        TimePoint previousFrameTime;
        TimePoint lastFPSTime;
        int frameCounter{};

        std::unordered_set<int> pressedKeys;

    public:
        double currentFPS{};
        std::string currentFPSText{};

        unsigned int width{};
        unsigned int height{};
        Core::IOEvent ioEvent{};

        App() = default;
        App(unsigned int width, unsigned int height) : width(width), height(height) {};
        ~App() = default;

        bool createWindow(const std::string &title);

        void dispose();

        // ----------------------------------------------------------
        // App construction sequence.
        // ----------------------------------------------------------

        // -------------------- 1st ----------------------------
        // Call by main
        int initialize();

        // -------------------- 2nd ----------------------------
        // Called by initialize.
        virtual void preSetup() = 0;

        // -------------------- 3rd ----------------------------
        /// @brief Called after App has initialized
        /// @return
        virtual int setup() = 0;

        // -------------------- 4th ----------------------------
        /// @brief Calls compile() which calls build()
        /// @return
        int configure();

        // -------------------- 4th A ----------------------------
        /// @brief Binds GLFW callbacks
        /// @return
        int setCallbacks();

        // -------------------- 4th B ----------------------------
        /// @brief Load GLAD functions
        /// @return
        int loadGlad();

        // -------------------- 4th C ----------------------------
        /// @brief Load GLAD functions
        /// @return
        void showOpenGLInfo();

        // -------------------- 4th D ----------------------------
        /// @brief Set GL Viewport
        /// @return
        int setViewport();

        // -------------------- 5th ----------------------------
        /// @brief Called when it is time to contruct your game nodes
        /// @return
        virtual int construct() = 0;

        // -------------------- 6th ----------------------------
        /// @brief Start game loop
        /// @return
        int run();

        // -------------------- 7th ----------------------------
        /// @brief Allows app to check if the app has been setup properly.
        ///        Called just prior to running game loop.
        /// @return
        virtual int verifyConfigured() = 0;

        /// @brief Called when the game loop exits and it is time to cleanup
        /// OpenGL.
        /// @return
        virtual int deconstruct() = 0;

        /// @brief Called every time an update is required
        /// @param dt
        /// @return
        virtual int update(double dt) = 0;

        /// @brief Called every frame to draw your game
        virtual void render() = 0;

        virtual void setupSmoothing();

        // -----------------------------------------------------------
        // Events
        // -----------------------------------------------------------
        virtual void processIOEvent() = 0;
        Core::IOEvent::Modifier modifierToEnum(int modifier);
        Core::IOEvent::Action actionToEnum(int action);
        Core::IOEvent::Button buttonToEnum(int button);

        // -----------------------------------------------------------
        // Events
        // -----------------------------------------------------------
        /// @brief Registers a key as being pressed.
        /// @param key The GLFW key code.
        void registerKeyPressed(int key)
        {
            pressedKeys.insert(key);
        }

        /// @brief Registers a key as being released.
        /// @param key The GLFW key code.
        void registerKeyReleased(int key)
        {
            pressedKeys.erase(key);
        }

        /// @brief Checks if a specific key is currently pressed.
        /// @param key The GLFW key code.
        /// @return True if the key is pressed, false otherwise.
        bool isKeyPressed(int key) const
        {
            return pressedKeys.count(key) > 0;
        }

        bool isKeyPressed(Core::IOEvent::Key key) const
        {
            return pressedKeys.count(static_cast<int>(key)) > 0;
        }
    };

} // namespace Game
