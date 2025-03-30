#include <iostream>

#include <vector2d.hpp>

void create_zero_vector()
{
    std::cout << "Testing zero vector" << std::endl;

    Core::Vector2D vector{};
    vector.x = 1.0;
    vector.y = 2.0;

    std::cout << "vector: " << vector << std::endl;

    // Sending 1 to exit() indicates the test failed.
    // std::exit(1);
}