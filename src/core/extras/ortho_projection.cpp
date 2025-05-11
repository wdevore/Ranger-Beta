#include <ortho_projection.hpp>

namespace Core
{
    void OrthoProjection::setProjection(double left, double right, double bottom, double top, double near, double far)
    {
        this->near = near;
        this->far = far;
        this->bottom = bottom;
        this->left = left;
        this->top = top;
        this->right = right;
        this->width = right - left;
        this->height = top - bottom;

        matrix.setToOrtho(left, right, bottom, top, near, far);
    }

    /// @brief Centers the projection and adjusts for aspect ratio.
    /// @param
    /// @param
    void OrthoProjection::setCenteredProjection(double near, double far)
    {
        // Adjust for aspect ratio
        left = -width / 2.0 / ratioCorrection;
        right = width / 2.0 / ratioCorrection;
        bottom = -height / 2.0 / ratioCorrection;
        top = height / 2.0 / ratioCorrection;

        matrix.setToOrtho(left, right, bottom, top, near, far);
    }

} // namespace Core
