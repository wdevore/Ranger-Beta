#pragma once

#include <iostream>

#include "matrix4.hpp"
#include "vector2d.hpp"

namespace Core
{
    static Vector2D tViewPoint_{};

    class Worldbase
    {
    private:
        /* data */
    public:
        Matrix4 viewSpace{};
        Matrix4 invViewSpace{};
        int deviceSizeWidth{0};
        int deviceSizeHeight{0};

        Worldbase(/* args */)
        {
            std::cout << "Worldbase Constructor" << std::endl;
        };

        ~Worldbase() = default;

        /// construct is called by the Engine during construction.
        /// You build your world in construct().
        virtual void construct() = 0;

        void end()
        {
            // Close node manager
        }

        // This is equivalent to toString()
        friend std::ostream &operator<<(std::ostream &os, const Worldbase &obj)
        {
            os << "[" << obj.deviceSizeWidth << " x " << obj.deviceSizeHeight << "]";
            return os;
        }
    };

} // namespace Core
