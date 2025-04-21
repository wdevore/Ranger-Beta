#pragma once

#include <glad/gl.h>
// We don't need to include opengl <gl.h> because glfw does it already.
#include <GLFW/glfw3.h>

#include <string>

#include <color4.hpp>

namespace Game
{
    class App
    {
    private:
    protected:
        /* data */
        GLFWwindow *window_;

        Color4 bgClearColor{0.2f, 0.3f, 0.3f, 1.0f};

    public:
        unsigned int width{};
        unsigned int height{};

        App() = default;
        App(unsigned int width, unsigned int height) : width(width), height(height) {};
        ~App();

        int initialize();
        bool createWindow(const std::string &title);
        int configure();

        /// @brief Start game loop
        /// @return
        int run();

        /// @brief Allows app to check if the app has been setup properly.
        /// @return
        virtual int verifyConfigured() = 0;

        /// @brief Called by configure so that derived classes can configure
        /// shaders and stuff.
        /// @return
        virtual int compile() = 0;
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
    };

} // namespace Game
