#pragma once

#include <string>
#include <vector>
#include <utility> // For std::pair
#include <limits>  // For double min/max values
#include <memory>
#include <ostream>

namespace Core
{
    using nlimits = std::numeric_limits<double>;
    using charVertices = std::vector<std::pair<double, double>>;
    using charVShPtr = std::shared_ptr<charVertices>;

    class CharVectors
    {
    private:
        /* data */
        std::string label;

    public:
        std::vector<charVShPtr> paths;
        charVShPtr currentPath;

        double minX{nlimits::infinity()};
        double maxX{-nlimits::infinity()};
        double width{0.0};

        double minY{nlimits::infinity()};
        double maxY{-nlimits::infinity()};
        double height{0.0};

        CharVectors(/* args */) = default;
        ~CharVectors() = default;

        void setLabel(const std::string &label);
        std::string getLabel() const;

        void newPath();
        void addVertex(double x, double y);

        // This is equivalent to toString()
        friend std::ostream &operator<<(std::ostream &os, const CharVectors &obj)
        {
            os << "'" << obj.label << "' (" << obj.paths.size() << "):";
            return os;
        }
    };

} // namespace Core
