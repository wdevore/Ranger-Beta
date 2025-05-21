#include <io_event.hpp>

namespace Core
{
    void IOEvent::setMouseWheelEvent(double x, double y)
    {
        x = x;
        y = y; // Vertical scroll amount and direction.
        type = Type::Scrolled;
    }

} // namespace Core
