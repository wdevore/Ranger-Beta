#pragma once

#include <glad/gl.h>

namespace Core
{
    /// @brief Viewport is a basic wrapper of an OpenGL viewport
    class Viewport
    {
    private:
        /* data */
        int x{};
        int y{};
        int width{};
        int height{};

    public:
        Viewport(/* args */) = default;
        ~Viewport() = default;

        /// @brief Set viewport dimensions
        /// @param x
        /// @param y
        /// @param width
        /// @param height
        void setDimensions(int x, int y, int width, int height);

        /// @brief Set the actual OpenGL viewport
        void apply();
    };

} // namespace Core
