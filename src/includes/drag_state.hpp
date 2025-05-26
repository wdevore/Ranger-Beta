#pragma once

#include <vector2d.hpp>
#include <environment.hpp>
#include <node.hpp>
#include <io_event.hpp>
#include <rectangle.hpp>

namespace Core
{
    class DragState
    {
    private:
        /* data */
        Vector2D panStartMouseInParentSpace_{};
        Vector2D panStartNodePosition_{};
        Vector2D currentMouseInParentSpace_{};
        Vector2D panDeltaInParentSpace_{};

        /// @brief Parent position mapped into Node's space
        Vector2D localPosition{};
        bool isDragLocked{false};

        /// @brief Node that will have dragging capabilities
        nodeShPtr node;
        /// @brief Used for hit testing
        Rectangle bounds{};

        environmentShPtr env;

        bool isPointInside_(double x, double y);

    public:
        bool pointInside{false};

        DragState() = default;
        ~DragState() = default;

        void initialize(nodeShPtr node, environmentShPtr environment, const Rectangle &bounds);
        bool handleEvent(const IOEvent &event);
    };

} // namespace Core
