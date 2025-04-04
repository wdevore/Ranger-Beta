#pragma once

#include <iostream>

namespace Core
{
    class Vector2D
    {
    private:
    public:
        double x{};
        double y{};

        Vector2D(/* args */) = default;
        Vector2D(double x, double y) : x(x), y(y) {}
        ~Vector2D();

        void setByAngle(double radians);

        double getLength();
        inline double getLengthSqr();
        void add(double x, double y);
        void add(const Vector2D &vector);
        void add(const Vector2D &v1, const Vector2D &v2, Vector2D &out);
        void sub(double x, double y);
        void sub(const Vector2D &vector);
        void sub(const Vector2D &v1, const Vector2D &v2, Vector2D &out);
        void scale(double s);
        void scale(double s, const Vector2D &v, Vector2D &out);
        void div(double s);

        void normalize();
        void setDirection(double radianAngle);
        double vectorDistance(const Vector2D &v1, const Vector2D &v2);
        double dot(const Vector2D &v1, const Vector2D &v2);
        double angleBetween(const Vector2D &v1, const Vector2D &v2);
        double cross(const Vector2D &v1, const Vector2D &v2);

        void toIdentity();

        // This is equivalent to toString()
        friend std::ostream &operator<<(std::ostream &os, const Vector2D &obj)
        {
            os << "<" << obj.x << "," << obj.y << ">";
            return os;
        }
    };

    static Vector2D tmpVec1_ = Vector2D{};
    static Vector2D tmpVec2_ = Vector2D{};

}
