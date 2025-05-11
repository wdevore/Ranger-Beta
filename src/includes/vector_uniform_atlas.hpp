#pragma once

#include "vector_atlas.hpp"

namespace Core
{
    class VectorObject;

    class VectorUniformAtlas final : public VectorAtlas
    {
    private:
        /* data */
    public:
        VectorUniformAtlas(/* args */) = default;
        ~VectorUniformAtlas() = default;

        void build(VectorObject *vo) override;

        void buildSquare_(VectorObject *vo);
    };

} // namespace Core
