#include <vector_uniform_atlas.hpp>

namespace Core
{
    /// @brief Adds vertext and indice
    /// @param x
    /// @param y
    void VectorUniformAtlas::add(float x, float y)
    {
        addVertex(x, y, 0.0f);
        addIndex(Idx_);
    }

    /// @brief Adds vertext and indice
    /// @param x
    /// @param y
    /// @param z
    void VectorUniformAtlas::add(float x, float y, float z)
    {
        addVertex(x, y, z);
        addIndex(Idx_);
    }

    /// @brief Adds vertext and indice
    /// @param x
    /// @param y
    /// @param z
    /// @param i
    void VectorUniformAtlas::add(float x, float y, float z, int i)
    {
        addVertex(x, y, z);
        addIndex(i);
    }

} // namespace Core
