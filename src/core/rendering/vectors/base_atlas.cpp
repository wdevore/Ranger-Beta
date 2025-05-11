#include <base_atlas.hpp>

namespace Core
{
    void BaseAtlas::initialize(int width, int height)
    {
        std::cout << "StaticMonoAtlas::initialize : " << width << "x" << height << std::endl;

        const float near{-1.0};
        const float far{1.0};
        const float w = static_cast<float>(width);
        const float h = static_cast<float>(height);

        // Origin at bottom-left
        // projection.setProjection(0.0, 0.0,
        //                          h, w,     // top-right
        //                          near, far);

        // Origin at screen center
        projection.setCenteredProjection(w, h, near, far);

        std::cout << "StaticMonoAtlas::initialize Projection matrix set" << std::endl;

        // |   0.00250,   0.00000,   0.00000,  -0.00000|
        // |   0.00000,   0.00333,   0.00000,  -0.00000|
        // |   0.00000,   0.00000,  -1.00000,   0.00000|
        // |   0.00000,   0.00000,   0.00000,   1.00000|
        // std::cout << projection.getMatrix() << std::endl;

        // An example of moving the camera (aka viewspace, aka viewMatrix)
        // camera.moveTo(50.0, 50.0);
    }

} // namespace Core
