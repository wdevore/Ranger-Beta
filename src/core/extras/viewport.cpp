#include <viewport.hpp>

namespace Core
{
    void Viewport::setDimensions(int x, int y, int width, int height)
    {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
    }

    void Viewport::apply()
    {
        glViewport(x, y, width, height);
    }

} // namespace Core
