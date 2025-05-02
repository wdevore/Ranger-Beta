#pragma once

#include <mesh.hpp>

namespace Core
{
    class VectorAtlas
    {
    private:
        /* data */

    protected:
        int vertexIdx_{};
        int vertexSize_{};

        int prevComponentCount_{};
        int componentCount_{};

        int Idx_{};
        int prevIndexCount_{};

    public:
        Mesh mesh{};

        VectorAtlas(/* args */) = default;
        ~VectorAtlas() = default;

        void construct();

        int addVertex(float x, float y, float z);
        void addIndex(int i);

        int begin();
        int end();
    };

} // namespace Core
