#include <base_atlas.hpp>
// #include <environment.hpp>

namespace Core
{
    void BaseAtlas::initialize(const int deviceWidth, const int deviceHeight)
    {
        // this->environment = environment;

        std::cout << "BaseAtlas::initialize : " << deviceWidth << "x" << deviceHeight << std::endl;

        const float near{-1.0};
        const float far{1.0};
        const float w = static_cast<float>(deviceWidth);
        const float h = static_cast<float>(deviceHeight);

        // -----------------------------------------------------------
        // Origin at bottom-left
        // -----------------------------------------------------------
        // projection.setProjection(0.0, 0.0,
        //                          h, w, // top-right
        //                          near, far);

        // -----------------------------------------------------------
        // Origin at top-left
        // This aligns with the device where the origin is in the upper-left.
        // -----------------------------------------------------------
        projection.setProjection(h, 0.0, // bottom-left
                                 0.0, w, // top-right
                                 near, far);

        // -----------------------------------------------------------
        // Origin at screen center
        // -----------------------------------------------------------
        // projection.setCenteredProjection(w, h, near, far);

        // std::cout << "BaseAtlas::initialize Projection matrix set" << std::endl;
        // |   0.00250,   0.00000,   0.00000,  -0.00000|
        // |   0.00000,   0.00333,   0.00000,  -0.00000|
        // |   0.00000,   0.00000,  -1.00000,   0.00000|
        // |   0.00000,   0.00000,   0.00000,   1.00000|
        // std::cout << projection.getMatrix() << std::endl;
    }

} // namespace Core
