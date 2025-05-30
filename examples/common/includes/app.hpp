#pragma once
#include <chrono>
#include <string>
#include <memory>

#include <glad/gl.h>
// We don't need to include opengl <gl.h> because glfw does it already.
#include <GLFW/glfw3.h>

#include <color4.hpp>
#include <io_event.hpp>
#include <game_environment.hpp>

namespace Game
{
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
        double currentFPS{};

    public:
        unsigned int width{};
        unsigned int height{};
        Core::IOEvent ioEvent{};

        gameEnvironmentShPtr environment = std::make_shared<GameEnvironment>();

        App() = default;
        App(unsigned int width, unsigned int height) : width(width), height(height) {};
        ~App() = default;

        int initialize();

        bool createWindow(const std::string &title);

        /// @brief Calls compile() which calls build()
        /// @return
        int configure();

        /// @brief Start game loop
        /// @return
        int run();

        // ----------------------------------------------------------
        // Virtuals
        // ----------------------------------------------------------
        virtual void preSetup() = 0;

        /// @brief Allows app to check if the app has been setup properly.
        /// @return
        virtual int verifyConfigured() = 0;

        /// @brief Called when the game loop exit and it is time to cleanup
        /// OpenGL.
        /// @return
        virtual int deconstruct() = 0;

        /// @brief Called after App has initialized
        /// @return
        virtual int setup() = 0;

        /// @brief Called when it is time to contruct your game nodes
        /// @return
        virtual int construct() = 0;

        /// @brief Called every time an update is required
        /// @param dt
        /// @return
        virtual int update(double dt) = 0;

        /// @brief Called every frame to draw your game
        virtual void render() = 0;

        // -----------------------------------------------------------
        // Events
        // -----------------------------------------------------------
        virtual void processIOEvent() = 0;
        Core::IOEvent::Modifier modifierToEnum(int modifier);
        Core::IOEvent::Action actionToEnum(int action);
        Core::IOEvent::Button buttonToEnum(int button);
    };

} // namespace Game
