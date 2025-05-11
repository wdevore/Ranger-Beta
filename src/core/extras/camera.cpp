#include <camera.hpp>

namespace Core
{
    void Camera::moveTo(float x, float y)
    {
        moveTo(x, y, 0.0);
    }

    void Camera::moveTo(float x, float y, float z)
    {
        // We move the camera in the opposite direction because
        // in reality we actually move the world.
        // For example, to make the camera look at 50,50 we translate
        // instead the world in the opposite direction, hence "-" prefix.
        viewspace.setTranslate(-x, -y, -z);
    }

} // namespace Core
