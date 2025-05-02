#pragma once

#include <string>
#include <memory>

#include <vector3.hpp>
#include <mesh.hpp>

namespace Core
{
    class VectorShape;
    using VectorShapeSPtr = std::shared_ptr<VectorShape>;

    class VectorShape final
    {
    private:
        // Offset is multiplied by the size of an Unsigned Int in preparation for
        // drawing.
        int offset_;

    public:
        std::string name{};
        GLuint primitiveType{};

        Vector3 minPoint{};
        Vector3 maxPoint{};

        int vertexCount;

        Mesh mesh{};

        VectorShape(/* args */) = default;
        ~VectorShape() = default;

        double width() const { return maxPoint.x - minPoint.x; }
        double height() const { return maxPoint.y - minPoint.y; }

        int offset() const;
        void setOffset(int offset);
    };

} // namespace Core
