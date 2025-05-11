#include "vector_atlas.hpp"

namespace Core
{
    void VectorAtlas::construct(VectorObject *vo)
    {
        // mesh.construct();
        build(vo);
    }

    int VectorAtlas::addVertex(float x, float y, float z)
    {
        // "x" is pushed first because the memory sequence is defined as:
        // xyzxyzxyz...
        mesh.vertices.push_back(x);
        mesh.vertices.push_back(y);
        mesh.vertices.push_back(z);

        // vertexIdx_ += vertexSize_;
        return componentCount_++;
    }

    int VectorAtlas::addVertices(const std::vector<float> &vertices)
    {
        for (auto &&v : vertices)
        {
            mesh.vertices.push_back(v);
        }

        componentCount_ += vertices.size() / 3;

        return componentCount_;
    }

    void VectorAtlas::addIndex(int i)
    {
        mesh.indices.push_back(i);
        // TODO this ++ should really find the max index and assign to _Idx.
        Idx_++;
    }

    void VectorAtlas::addIndex(const std::vector<int> &indices)
    {
        for (auto &&i : indices)
        {
            mesh.indices.push_back(i);
        }

        Idx_ += indices.size() - 1;
    }

    int VectorAtlas::begin()
    {
        prevComponentCount_ = componentCount_;
        prevIndexCount_ = Idx_;
        return prevIndexCount_;
    }

    int VectorAtlas::end()
    {
        // int count = _componentCount - _prevComponentCount;
        int count = Idx_ - prevIndexCount_;
        return count;
    }

} // namespace Core
