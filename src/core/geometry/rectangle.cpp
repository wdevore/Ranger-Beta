#include <limits>
#include <cmath>

#include "rectangle.hpp"

namespace Core
{
    void Rectangle::set(const Rectangle &rec)
    {
        bottom = rec.bottom;
        left = rec.left;
        top = rec.top;
        right = rec.right;
        width = rec.width;
        height = rec.height;
    }

    void Rectangle::set(double width, double height)
    {
        // bottom = 0.0;  // This is if +Y is upward.
        // left = 0.0;
        // top = height;
        // right = width;
        bottom = height;
        left = 0.0;
        top = 0.0;
        right = width;
        this->width = width;
        this->height = height;
    }

    void Rectangle::set(double minX, double minY, double maxX, double maxY)
    {
        left = minX;
        right = maxX;
        top = minY;
        bottom = maxY;
        width = maxX - minX;
        height = maxY - minY;
    }

    /// @brief Expands/Fits rectangle to fit vertex cloud and set center.
    ///        vector is structured as: [x,y,x,y...]
    /// @param vertices
    void Rectangle::set(const std::vector<double> &vertices)
    {
        using nlimits = std::numeric_limits<double>;

        double minX = nlimits::infinity();
        double minY = nlimits::infinity();
        double maxX = -nlimits::infinity();
        double maxY = -nlimits::infinity();

        // Find min/max points
        for (double i = 0; i < vertices.size(); i += 2)
        {
            double x = vertices[i];
            double y = vertices[i + 1];

            minX = fmin(minX, x);
            minY = fmin(minY, y);

            maxX = fmax(maxX, x);
            maxY = fmax(maxY, y);
        }

        set(minX, minY, maxX, maxY);
        setCenter(left + width / 2.0, bottom + height / 2.0);
    }

    void Rectangle::setCenter(double x, double y)
    {
        center.set(x, y);
    }

    void Rectangle::setSize(double w, double h)
    {
        width = w;
        height = h;
    }

    void Rectangle::expand(double wx, double wy)
    {
        double minX = left;
        double minY = bottom;
        double maxX = right;
        double maxY = top;

        minX = fmin(minX, wx);
        minY = fmin(minY, wy);

        maxX = fmax(maxX, wx);
        maxY = fmax(maxY, wy);

        set(minX, minY, maxX, maxY);
        setCenter(left + width / 2.0, bottom + height / 2.0);
    }

    double Rectangle::area() const
    {
        return width * height;
    }

    /// @brief Checks point (Vector2D) using left-top rule.
    ///
    /// Rule: Point is inside if on an top or left Edge and **NOT** on a bottom
    /// or right edge
    /// @param p
    /// @return
    bool Rectangle::pointContained(const Vector2D &p) const
    {
        // Because the Canvas's +Y is downward the bottom is positive so the check is:
        // y is < bottom and y is >= top.
        // std::cout << (p.x > left) << std::endl;
        // std::cout << (p.x < right) << std::endl;
        // std::cout << (p.y < bottom) << std::endl;
        // std::cout << (p.y > top) << std::endl;

        return p.x > left && p.x < right && p.y < bottom && p.y > top;
    }

    /// @brief Checks point using left-top rule.
    ///
    /// Rule: Point is inside if on an top or left Edge and **NOT** on a bottom
    /// or right edge
    /// @param p
    /// @return
    bool Rectangle::pointInside(const Vector2D &p) const
    {
        // Because the Canvas's +Y is downward the bottom is positive so the check is:
        // y is < bottom and y is >= top.
        return coordsInside(p.x, p.y);
    }

    bool Rectangle::coordsInside(double x, double y) const
    {
        //  Canvas's device coordinate space is:
        //        Top-Left
        //         (0,0)
        //           .--------> +X
        //           |
        //           |
        //           |
        //           v         Bottom-Right
        //          +Y
        //      Lower-Left
        //
        // If +Y is upward
        // return x >= left && x < right && y >= bottom && y < top;
        //
        // Because the Canvas's +Y is downward the bottom is positive so the check is:
        // y is < bottom and y is >= top.
        std::cout << (x >= left) << std::endl;
        std::cout << (x < right) << std::endl;
        std::cout << (y < bottom) << std::endl;
        std::cout << (y >= top) << std::endl;

        return x >= left && x < right && y < bottom && y >= top;
    }

    /// @brief returns *true* if other 'r' intersects this rectangle.
    /// @param r
    /// @return
    bool Rectangle::intersects(const Rectangle &r) const
    {
        return left <= r.right &&
               r.left <= right &&
               top <= r.bottom &&
               r.top <= bottom;
        // return left <= o.right &&
        //     o.left <= right &&
        //     bottom <= o.top &&
        //     o.bottom <= top;
    }

    /// @brief returns *true* if rectangle completely contains other (r).
    /// @param r
    /// @return
    bool Rectangle::contains(const Rectangle &r) const
    {
        return left <= r.left &&
               right >= r.right &&
               top <= r.top &&
               bottom >= r.bottom;
    }

} // namespace Core
