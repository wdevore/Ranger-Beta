#pragma once

#include <vector>

#include <vector2d.hpp>

namespace Core
{
    /// Rectangle is a square with behaviours.
    /// It matches [OpenGL].
    /// ```
    ///     minX,minY      >
    ///          .--------------------.
    ///          |        Top         |
    ///          |                    |
    ///          |                    |
    ///       >  | Left    .   right  |  <
    ///          |                    |
    ///          |                    |
    ///          |      bottom        |
    ///          .--------------------.
    ///                     <     maxX, maxY
    ///```
    ///
    class Rectangle final
    {
    private:
        /* data */
    public:
        float left{0.0};
        float top{0.0};
        float bottom{0.0};
        float right{0.0};
        float width{0.0};
        float height{0.0};
        Vector2D center{};

        Rectangle(/* args */) = default;
        ~Rectangle() = default;

        void set(const Rectangle &rec);
        void set(float width, float height);
        void set(float minX, float minY, float maxX, float maxY);
        void set(const std::vector<float> &vertices);
        void setCenter(float x, float y);
        void setSize(float w, float h);

        void expand(float wx, float wy);
        float area() const;
        bool pointContained(const Vector2D &p) const;
        bool pointInside(const Vector2D &p) const;
        bool coordsInside(float x, float y) const;
        bool intersects(const Rectangle &r) const;
        bool contains(const Rectangle &r) const;

        // This is equivalent to toString()
        friend std::ostream &operator<<(std::ostream &os, const Rectangle &obj)
        {
            os << "TL: (" << obj.left << "," << obj.top << ")"
               << " RB: (" << obj.right << "," << obj.bottom << ")"
               << " [" << obj.width << " x " << obj.height << "]\n";
            return os;
        }
    };

} // namespace Core
