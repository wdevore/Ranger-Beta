#include <iostream>

#include "vector2d.hpp"

namespace Core
{
    Vector2D::~Vector2D()
    {
        std::cout << "Vector2D destructor called." << std::endl;
    }
}
