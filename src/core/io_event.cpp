#include <iostream>

#include <io_event.hpp>

namespace Core
{
    void IOEvent::reset()
    {
        type = Type::None;
        action = Action::Unknown;
        button = Button::Unknown;
        isDragging = false;
        isDragStart = false;
        isDragDown = false;
        isDragEnd = false;
    }

    void IOEvent::setMouseWheelEvent(double x, double y)
    {
        reset();
        this->x = x;
        this->y = y; // Vertical scroll amount and direction.
        type = Type::Scrolled;
    }

    void IOEvent::setMouseMoveEvent(double x, double y)
    {
        reset();
        this->x = x;
        this->y = y;
        type = Type::Moved;

        if (isDragStart || isDragging)
        {
            // std::cout << "ioevent move" << std::endl;
            isDragging = true;
            isDragDown = true;
            dx = x - px;
            dy = y - py;
            px = x;
            py = y;
        }
    }

    void IOEvent::setMouseEnterExitEvent(bool entered)
    {
        reset();
        type = entered ? Type::Entered : Type::Left;
    }

    void IOEvent::setMouseButtonEvent(Button button, Action action, Modifier mods)
    {
        reset();
        this->button = button;
        this->action = action;
        this->mods = mods;

        if (action == IOEvent::Action::Press)
        {
            // std::cout << "ioevent drag start" << std::endl;
            isDragging = true;
            isDragStart = true;
            dx = dy = 0.0;
            px = x;
            py = y;
            // Position is already set from MoveEvent
        }
        else if (action == IOEvent::Action::Release)
        {
            if (isDragging)
                isDragEnd = true; // Only is true dragging occurred.
            isDragStart = false;
            isDragging = false;
        }
    }

    void IOEvent::setKeyboardEvent(int key, int scancode, Action action, Modifier mods)
    {
        reset();
        this->type = Type::Key;
        this->key = key;
        this->scancode = scancode;
        this->action = action;
        this->mods = mods;
    }

} // namespace Core
