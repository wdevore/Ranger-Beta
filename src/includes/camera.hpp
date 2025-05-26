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
        Matrix4 invViewSpace{true};

        Camera(/* args */) {
            // viewspace.setTranslate(-400.0f, 200.0f, 0.0f);
        };
        ~Camera() = default;

        void moveTo(float x, float y);
        void moveTo(float x, float y, float z);
    };

} // namespace Core
