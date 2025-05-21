#pragma once

namespace Core
{
    class IOEvent
    {
    private:
        /* data */
    public:
        // Enum to define the type of mouse event
        enum class Type
        {
            None,
            ButtonPressed,
            ButtonReleased,
            // Add ButtonRepeated here if you want to handle GLFW_REPEAT explicitly
            Moved,
            Scrolled,
            Entered, // Cursor entered the window client area
            Left     // Cursor left the window client area
        };

        // Offset/Position data (relevant for most events)
        double x{};
        double y{};

        Type type{Type::None};

        IOEvent(/* args */) = default;
        ~IOEvent() = default;

        void setMouseWheelEvent(double x, double y);
    };

} // namespace Core
