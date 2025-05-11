#pragma once

#include "matrix4.hpp"

namespace Core
{
    class OrthoProjection
    {
    private:
        /* data */
        double near{};
        double far{};

        double left{};
        double right{};
        double bottom{};
        double top{};

        double width{};
        double height{};

        double ratioCorrection{};

        // Projection matrix (orthographic)
        Matrix4 matrix{true};

    public:
        OrthoProjection(/* args */) = default;
        ~OrthoProjection() = default;

        const Matrix4 &getMatrix() const { return matrix; };

        void setProjection(double left, double right,
                           double bottom, double top,
                           double near, double far);

        void setCenteredProjection(double near, double far);
    };

} // namespace Core
