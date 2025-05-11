#pragma once

#include <unordered_map>
#include <vector>

#include "mesh.hpp"
#include "vector_shape.hpp"

namespace Core
{
    class VectorObject;

    class VectorAtlas
    {
    private:
        /* data */

    protected:
        // int vertexIdx_{};
        // int vertexSize_{};

        int prevComponentCount_{};
        int componentCount_{};

        int Idx_{};
        int prevIndexCount_{};

    public:
        std::unordered_map<std::string, VectorShapeSPtr> shapes{};
        Mesh mesh{};

        VectorAtlas(/* args */) = default;
        ~VectorAtlas() = default;

        void construct(VectorObject *vo);

        int addVertex(float x, float y, float z);
        int addVertices(const std::vector<float> &vertices);
        void addIndex(int i);
        void addIndex(const std::vector<int> &indices);

        virtual void build(VectorObject *vo) = 0;

        int begin();
        int end();
    };

} // namespace Core
