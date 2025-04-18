#include <cmath>

#include "char_vectors.hpp"

namespace Core
{
    void CharVectors::setLabel(const std::string &label)
    {
        this->label = label;
    }

    std::string CharVectors::getLabel() const
    {
        return label;
    }

    void CharVectors::newPath()
    {
        currentPath = std::make_shared<charVertices>();
        paths.push_back(currentPath);
    }

    void CharVectors::addVertex(double x, double y)
    {
        minX = std::min(minX, x);
        maxX = std::max(maxX, x);
        width = fabs(maxX - minX);

        minY = std::min(minY, y);
        maxY = std::max(maxY, y);
        height = fabs(maxY - minY);

        // currentPath->push_back(std::make_pair(x, y));
        // OR
        currentPath->push_back({x, y});
    }

} // namespace Core
