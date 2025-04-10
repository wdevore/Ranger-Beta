#include <transform.hpp>

namespace Core
{
    Transform::Transform(/* args */)
    {
        aft.toIdentity();
        inverse.toIdentity();
    }

    void Transform::calcFilteredTransform(bool excludeTranslation, bool excludeRotation, bool excludeScale, AffineTransform &aft)
    {
        aft.toIdentity();

        if (!excludeTranslation)
        {
            aft.makeTranslate(position.x, position.y);
        }

        if (!excludeRotation && rotation != 0.0)
        {
            aft.rotate(rotation);
        }

        if (!excludeScale && (scale.x != 0.0 || scale.y != 0.0))
        {
            aft.scale(scale.x, scale.y);
        }
    }

} // namespace Core
