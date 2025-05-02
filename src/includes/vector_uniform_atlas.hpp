#pragma once

#include <vector_atlas.hpp>

namespace Core
{
    class VectorUniformAtlas final : public VectorAtlas
    {
    private:
        /* data */
    public:
        VectorUniformAtlas(/* args */) = default;
        ~VectorUniformAtlas() = default;

        void add(float x, float y);
        void add(float x, float y, float z);

        void add(float x, float y, float z, int i);
    };

} // namespace Core
