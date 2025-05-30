#pragma once

namespace Core
{
    /// @brief A delay in milliseconds.
    class Delay final
    {
    private:
        /* data */
        double timeCnt_{};
        double duration_{}; // in milliseconds

    public:
        Delay() = default;
        Delay(double duration) : duration_(duration) {};
        ~Delay() = default;

        void reset();
        /// @brief Return 'true' if delay expired.
        /// @param dt
        /// @return bool
        bool expired(double dt);
    };

} // namespace Core
