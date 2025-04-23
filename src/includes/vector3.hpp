#pragma once

#include <iostream>

namespace Core
{
    class Matrix4; // Forward decleration to avoid cyclic dependency.

    class Vector3 final
    {
    private:
        /* data */
    public:
        double x{};
        double y{};
        double z{};

        // We MUST provide a default constructor OR one that initializes 1
        // or more members otherwise the contstructor is deleted.
        // Vector3(/* args */) : x(0), y(0), z(0) {}
        // OR
        Vector3() = default;

        Vector3(double x, double y, double z) : x(x), y(y), z(z) {}
        // We MUST declare a default destructor because an empty
        // destructor is deleted causing the linker to complain.
        ~Vector3() = default;

        void set(double x, double y, double z);
        void set(double x, double y);
        void set(const Vector3 &vector);

        void add(double x, double y);
        void add(const Vector3 &vector);
        void add(const Vector3 &v1, const Vector3 &v2, Vector3 &out);

        void sub(const Vector3 &vector);
        void sub(const Vector3 &v1, const Vector3 &v2, Vector3 &out);

        void uniformScale(double s);
        void nonUniformScale(double s);
        void mulScaleAdd(const Vector3 &vector, double scalar);
        double getLength() const;
        double getLengthSqr() const;
        bool equal(const Vector3 &vector) const;
        double distance(const Vector3 &vector) const;
        double distanceSquared(const Vector3 &vector) const;
        double dot(const Vector3 &vector);
        void cross(const Vector3 &vector);
        void mul(const Matrix4 &m);

        Vector3 clone(const Vector3 &vector) const;

        // This is equivalent to toString()
        friend std::ostream &operator<<(std::ostream &os, const Vector3 &obj)
        {
            os << "<" << obj.x << "," << obj.y << "," << obj.z << ">";
            return os;
        }
    };

}