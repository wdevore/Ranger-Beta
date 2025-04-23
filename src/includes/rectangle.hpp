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
        double left{0.0};
        double top{0.0};
        double bottom{0.0};
        double right{0.0};
        double width{0.0};
        double height{0.0};
        Vector2D center{};

        Rectangle(/* args */) = default;
        ~Rectangle() = default;

        void set(const Rectangle &rec);
        void set(double width, double height);
        void set(double minX, double minY, double maxX, double maxY);
        void set(const std::vector<double> &vertices);
        void setCenter(double x, double y);
        void setSize(double w, double h);

        void expand(double wx, double wy);
        double area() const;
        bool pointContained(const Vector2D &p) const;
        bool pointInside(const Vector2D &p) const;
        bool coordsInside(double x, double y) const;
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
