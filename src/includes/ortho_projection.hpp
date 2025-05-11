#pragma once

#include "matrix4.hpp"

namespace Core
{
    class OrthoProjection
    {
    private:
        /* data */
        float near{};
        float far{};

        float left{};
        float right{};
        float bottom{};
        float top{};

        float width{};
        float height{};

        float ratioCorrection{1.0};

        // Projection matrix (orthographic)
        Matrix4 matrix{true};

    public:
        OrthoProjection(/* args */) = default;
        ~OrthoProjection() = default;

        const Matrix4 &getMatrix() const { return matrix; };

        void setProjection(float bottom, float left,
                           float top, float right,
                           float near, float far);

        void setCenteredProjection(float width, float height, float near, float far);
    };

} // namespace Core
