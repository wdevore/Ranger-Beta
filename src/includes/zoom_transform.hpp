#pragma once

#include "vector2d.hpp"
#include "affinetransform.hpp"

namespace Core
{
    class ZoomTransform
    {
    private:
        /* data */
        // An optional (occasionally used) translation.
        Vector2D position{};

        // The zoom factor generally incremented in small steps.
        // For example, 0.1
        Vector2D scale{1.0, 1.0};

        // The focal point where zooming occurs
        Vector2D zoomAt{};

        // A "running" accumulating transform
        AffineTransform accTransform{true};

        // A transform that includes position translation.
        AffineTransform transform{true};

    public:
        ZoomTransform(/* args */) = default;
        ~ZoomTransform() = default;

        void update();
        void setPosition(float x, float y);
        void zoomBy(float dx, float dy);
        void translateBy(float dx, float dy);
        /// @brief Returns X component.
        /// @return
        float getScale() const;
        float getPseudoScale() const;
        void setScale(float scale);
        void setAt(float x, float y);

        AffineTransform &getTransform();
    };

} // namespace Core
