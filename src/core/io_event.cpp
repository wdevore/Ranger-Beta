#include <io_event.hpp>

namespace Core
{
    void IOEvent::setMouseWheelEvent(double x, double y)
    {
        this->x = x;
        this->y = y; // Vertical scroll amount and direction.
        type = Type::Scrolled;
    }

    void IOEvent::setMouseMoveEvent(double x, double y)
    {
        this->x = x;
        this->y = y;
        type = Type::Moved;
    }

    void IOEvent::setMouseEnterExitEvent(bool entered)
    {
        type = entered ? Type::Entered : Type::Left;
    }

    void IOEvent::setMouseButtonEvent(Button button, Action action, Modifier mods)
    {
        this->button = button;
        this->action = action;
        this->mods = mods;
    }

    void IOEvent::setKeyboardEvent(int key, int scancode, Action action, Modifier mods)
    {
        this->type = Type::Key;
        this->key = key;
        this->scancode = scancode;
        this->action = action;
        this->mods = mods;
    }

} // namespace Core
