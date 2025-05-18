#include <environment.hpp>

namespace Core
{
    void Environment::initialize()
    {
        atlas.initialize(shared_from_this());
    }

} // namespace Core
