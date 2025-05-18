#include <environment.hpp>

namespace Core
{
    void Environment::initialize(int deviceWidth, int deviceHeight)
    {
        this->deviceWidth = deviceWidth;
        this->deviceHeight = deviceHeight;

        atlas.initialize(shared_from_this());
    }

    void Environment::postInitialize()
    {
        atlas.burn();
        atlas.use();
    }

} // namespace Core
