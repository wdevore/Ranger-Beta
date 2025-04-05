#include <iostream>
#include <cmath>

#include <vector2d.hpp>
#include <constants.hpp>

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

    void Vector2D::set(double x, double y)
    {
        this->x = x;
        this->y = y;
    }

    void Vector2D::set(const Vector2D &vector)
    {
        x = vector.x;
        y = vector.y;
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

    void Vector2D::add(const Vector2D &v1, const Vector2D &v2, Vector2D &out)
    {
        out.set(v1.x + v2.x, v1.y + v2.y);
    }

    void Vector2D::sub(double x, double y)
    {
        this->x -= x;
        this->y -= y;
    }
    /// @brief Decrement by a vector
    /// @param vector
    void Vector2D::sub(const Vector2D &vector)
    {
        x -= vector.x;
        y -= vector.y;
    }

    /// @brief out = v1 - v2
    /// @param v1 Vector2D
    /// @param v2 Vector2D
    /// @param out Vector2D
    void Vector2D::sub(const Vector2D &v1, const Vector2D &v2, Vector2D &out)
    {
        out.x = v1.x - v2.x;
        out.y = v1.y - v2.y;
    }

    void Vector2D::scale(double s)
    {
        x *= s;
        y *= s;
    }

    void Vector2D::scale(double s, const Vector2D &v, Vector2D &out)
    {
        out.x = s * v.x;
        out.y = s * v.y;
    }

    void Vector2D::div(double s)
    {
        x /= s;
        y /= s;
    }

    void Vector2D::normalize()
    {
        double len = getLength();
        if (len != 0.0)
            div(len);
    }

    void Vector2D::setDirection(double radianAngle)
    {
        x = cos(radianAngle);
        y = sin(radianAngle);
    }

    double Vector2D::vectorDistance(const Vector2D &v1, const Vector2D &v2)
    {
        sub(v1, v2, tmpVec1_);
        return tmpVec1_.getLength();
    }

    double Vector2D::dot(const Vector2D &v1, const Vector2D &v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    double Vector2D::angleBetween(const Vector2D &v1, const Vector2D &v2)
    {
        tmpVec1_.set(v1);
        tmpVec2_.set(v2);

        tmpVec1_.normalize();
        tmpVec2_.normalize();

        double angl = atan2(cross(tmpVec1_, tmpVec2_), dot(tmpVec1_, tmpVec2_));

        if (fabs(angl) < EPSILON)
            return 0.0;

        return angl;
    }

    double Vector2D::cross(const Vector2D &v1, const Vector2D &v2)
    {
        return v1.x * v2.y - v1.y * v2.x;
    }

    void Vector2D::toIdentity()
    {
        x = 1.0;
        y = 1.0;
    }
}
