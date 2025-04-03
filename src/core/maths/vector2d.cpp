#include <iostream>
#include <cmath>

#include "vector2d.hpp"

namespace Core
{
    Vector2D::~Vector2D()
    {
        std::cout << "Vector2D destructor called." << std::endl;
    }

    void Vector2D::setByAngle(double radians)
    {
        x = cos(radians);
        y = sin(radians);
    }

    double Vector2D::getLength()
    {
        return sqrt(getLengthSqr());
    }

    inline double Vector2D::getLengthSqr()
    {
        return x * x + y * y;
    }

    /// @brief Increments this vector by constants
    /// @param x
    /// @param y
    void Vector2D::add(double x, double y)
    {
        this->x += x;
        this->y += y;
    }

    /// @brief Increments by a vector
    /// @param vector
    void Vector2D::add(const Vector2D &vector)
    {
        x += vector.x;
        y += vector.y;
    }

    void Vector2D::sub(double x, double y)
    {
        this->x -= x;
        this->y -= y;
    }
}
