
#include <constants.hpp>

namespace Core
{
    std::ostream &operator<<(std::ostream &os, ErrorConditions s)
    {
        switch (s)
        {
        case ErrorConditions::None:
            os << "None";
            break;
        case ErrorConditions::UnknownSignal:
            os << "UnknownSignal";
            break;
        case ErrorConditions::AttemptingToRemoveNonTopNode:
            os << "AttemptingToRemoveNonTopNode";
            break;
        default:
            os << "Unknown ErrorConditions";
            break;
        }
        return os;
    }

} // namespace Core
