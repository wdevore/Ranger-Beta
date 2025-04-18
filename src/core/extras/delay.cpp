#include "delay.hpp"

namespace Core
{
    void Delay::reset()
    {
        timeCnt_ = 0.0;
    }

    bool Delay::expired(double dt)
    {
        timeCnt_ += dt;
        return timeCnt_ > duration_;
    }

} // namespace Core
