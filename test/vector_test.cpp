#include <iostream>

#include <vector2d.hpp>

void create_zero_vector()
{
    std::cout << "Testing zero vector" << std::endl;

    Core::Vector2D vector{};
    vector.x = 1.0;
    vector.y = 2.0;

    std::cout << "vector: " << vector << std::endl;

    Core::Vector2D vectorInitList{3, 5};
    if (!(vectorInitList.x == 3 && vectorInitList.y == 5))
    {
        std::cout << "ERROR: vectorInitList not <3,4> got: " << vectorInitList << std::endl;
        std::exit(1);
    }

    double len = vector.getLength();
    std::cout << "vector len: " << len << std::endl;

    vector.add(1.0, 1.0);

    std::cout << "vector add: " << vector << std::endl;

    std::cout << "vectorInitList: " << vectorInitList << std::endl;

    // Sending 1 to exit() indicates the test failed.
    // std::exit(1);
}