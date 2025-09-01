#include <environment.hpp>

namespace Core
{
    void Environment::initialize(int deviceWidth, int deviceHeight)
    {
        this->deviceWidth = deviceWidth;
        this->deviceHeight = deviceHeight;

        atlas.initialize(shared_from_this());

        // An example of moving the camera (aka viewspace, aka viewMatrix)
        // camera.moveTo(-200.0, -150.0);
    }

    /// @brief Called by GameApp construct
    void Environment::postInitialize()
    {
        atlas.burn();
    }

    void Environment::dispose()
    {
        atlas.unUse();
        atlas.dispose();
        pressedKeys.clear();
    }

    // void Environment::setFontAtlas(bvFontAtlasUnqPtr fontAtlas)
    // {
    //     this->fontAtlas = std::move(fontAtlas);
    // }

} // namespace Core
