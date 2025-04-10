#include <vector2d.hpp>
#include <affinetransform.hpp>

namespace Core
{
    class Transform
    {
    private:
        /* data */
    public:
        Vector2D position{};
        double rotation{0};
        Vector2D scale{1.0, 1.0};

        AffineTransform aft{};
        AffineTransform inverse{};

        Transform(/* args */);
        ~Transform() = default;

        void calcFilteredTransform(bool excludeTranslation,
                                   bool excludeRotation,
                                   bool excludeScale,
                                   AffineTransform &aft);
    };

} // namespace Core
