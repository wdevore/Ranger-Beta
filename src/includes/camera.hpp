#pragma once

#include "matrix4.hpp"

namespace Core
{
    class Camera
    {
    private:
        /* data */

    public:
        Matrix4 viewspace{true};

        Camera(/* args */) = default;
        ~Camera() = default;

        void moveTo(float x, float y);
        void moveTo(float x, float y, float z);
    };

} // namespace Core
