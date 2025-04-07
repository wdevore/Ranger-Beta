#include <cmath>
#include <iostream>

#include <constants.hpp>

/// @brief Return true if the difference between 'expected' and 'actual' >
///        EPSILON.
/// @param expected double
/// @param actual double
/// @return bool
bool epsilonCheck(double expected, double actual)
{
    double dif = fabs(expected - actual);
    std::cout << "epsilonCheck::dif: " << dif << std::endl;

    return dif > Core::EPSILON;
}

void checkShow(const std::string &msg, double expected, double actual)
{
    std::cout << "ERROR: Expected " << msg << expected << " -> Got  " << actual << std::endl;
}