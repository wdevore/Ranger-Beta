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

    AffineTransform &Transform::calcTransform(bool dirty)
    {
        if (dirty)
        {
            aft.makeTranslate(position.x, position.y);

            if (rotation != 0.0)
            {
                aft.rotate(rotation);
            }

            if (scale.x != 1.0 || scale.y != 1.0)
            {
                aft.scale(scale.x, scale.y);
            }

            // Invert...
            aft.invert(inverse);
        }

        return aft;
    }

} // namespace Core
