#pragma once

#include <cmath>
#include <iostream>

namespace Core
{
    static const double EPSILON = 0.00001;
    static const double DEGREES_TO_RADIANS = M_PI / 180.0;
    static const double RADIANS_TO_DEGREES = 180.0 / M_PI;

    enum class ErrorConditions
    {
        /// @brief No Error occurred.
        None,
        /// @brief A signal was either sent or received that wasn't recognized
        UnknownSignal,
        /// @brief Attempt to remove Node from stage that wasn't at the top
        AttemptingToRemoveNonTopNode
    };

    std::ostream &operator<<(std::ostream &os, ErrorConditions s);
}