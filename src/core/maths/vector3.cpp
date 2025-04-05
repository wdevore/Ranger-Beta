#include <cmath>

#include <vector3.hpp>
#include <matrix4.hpp>

#include <constants.hpp>

namespace Core
{
    // Vector3::Vector3(/* args */) {};

    // Vector3::~Vector3()
    // {
    // }

    void Vector3::set(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void Vector3::set(double x, double y)
    {
        this->x = x;
        this->y = y;
        this->z = 0.0;
    }

    void Vector3::set(const Vector3 &vector)
    {
        x = vector.x;
        y = vector.y;
        z = vector.z;
    }

    void Vector3::add(double x, double y)
    {
        this->x += x;
        this->y += y;
        this->z += z;
    }

    void Vector3::add(const Vector3 &vector)
    {
        x += vector.x;
        y += vector.y;
        z += vector.z;
    }

    void Vector3::add(const Vector3 &v1, const Vector3 &v2, Vector3 &out)
    {
        out.set(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
    }

    void Vector3::sub(const Vector3 &vector)
    {
        x -= vector.x;
        y -= vector.y;
        z -= vector.z;
    }

    /// @brief out = v1 - v2
    /// @param v1 Vector3
    /// @param v2 Vector3
    /// @param out Vector3
    void Vector3::sub(const Vector3 &v1, const Vector3 &v2, Vector3 &out)
    {
        out.set(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
    }

    void Vector3::uniformScale(double s)
    {
        x *= s;
        y *= s;
        z *= s;
    }

    /// @brief Z component is not scaled
    /// @param s
    void Vector3::nonUniformScale(double s)
    {
        x *= s;
        y *= s;
    }

    /// @brief "this" += vector * scalar
    /// @param vector
    /// @param scalar
    void Vector3::mulScaleAdd(const Vector3 &vector, double scalar)
    {
        x += vector.x * scalar;
        y += vector.y * scalar;
        z += vector.z * scalar;
    }

    double Vector3::getLength()
    {
        return sqrt(getLengthSqr());
    }

    inline double Vector3::getLengthSqr()
    {
        return x * x + y * y;
    }

    /// @brief Epsilon equality
    /// @param vector
    /// @return bool
    bool Vector3::equal(const Vector3 &other)
    {
        return (fabs(x - other.x) <= Core::EPSILON) &&
               (fabs(y - other.y) <= Core::EPSILON) &&
               (fabs(z - other.z) <= Core::EPSILON);
    }

    /// @brief the euclidean distance between the two specified vectors
    /// @param vector Vector3
    /// @return double
    double Vector3::distance(const Vector3 &vector)
    {
        return sqrt(distanceSquared(vector));
    }

    inline double Vector3::distanceSquared(const Vector3 &vector)
    {
        double a = vector.x - x;
        double b = vector.y - y;
        double c = vector.z - z;

        return a * a + b * b + c * c;
    }

    double Vector3::dot(const Vector3 &vector)
    {
        return x * vector.x + y * vector.y + z * vector.z;
    }

    /// @brief "This" <-- "This" cross vector
    /// @param vector Vector3
    /// @return double
    void Vector3::cross(const Vector3 &vector)
    {
        x = y * vector.z - z * vector.y;
        y = z * vector.x - x * vector.z;
        z = x * vector.y - y * vector.x;
    }

    /// @brief left-multiplies the vector by the given matrix,
    ///        assuming the fourth (w) component of the vector is 1.
    ///
    ///     ```
    ///     [M00 M01 M02 M03]   [x]
    ///     [M10 M11 M12 M13] x [y]
    ///     [M20 M21 M22 M23]   [z]
    ///     [M30 M31 M32 M33]   [1]  <-- w
    ///     ```
    /// @param m Matrix4
    void Vector3::mul(const Matrix4 &m)
    {
        x = x * m.e[Matrix4::m00] +
            y * m.e[Matrix4::m01] +
            z * m.e[Matrix4::m02] +
            m.e[Matrix4::m03];
        y = x * m.e[Matrix4::m10] +
            y * m.e[Matrix4::m11] +
            z * m.e[Matrix4::m12] +
            m.e[Matrix4::m13];
        z = x * m.e[Matrix4::m20] +
            y * m.e[Matrix4::m21] +
            z * m.e[Matrix4::m22] +
            m.e[Matrix4::m23];
    }

    Vector3 Vector3::clone(const Vector3 &vector)
    {
        return Vector3{vector.x, vector.y, vector.z};
    }

}