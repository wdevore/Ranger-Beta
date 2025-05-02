#pragma once

#include "vao.hpp"
#include "vector_uniform_atlas.hpp"

namespace Core
{
    class VectorObject
    {
    private:
        /* data */

    public:
        Vao vao{};
        VectorUniformAtlas atlas{};

        VectorObject(/* args */) = default;
        ~VectorObject() = default;

        void construct();

        void use();

        void unUse();

        void release();

        void bind();
    };

} // namespace Core
