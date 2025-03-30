#pragma once

#include <iostream>
#include <string>

namespace Core
{
    class Vector2D
    {
    private:
        // static std::unique_ptr<Vector2D> tmp = std::make_unique<Vector2D>();

    public:
        double x{};
        double y{};

        Vector2D(/* args */) = default;
        // Don't declare this here unless you are going to make it "virtual"
        // in relation to polymorphism.
        // ~Vector2D();

        // This is equivalent to toString()
        friend std::ostream &operator<<(std::ostream &os, const Vector2D &obj)
        {
            os << "<" << obj.x << "," << obj.y << ">";
            return os;
        }
    };

}
