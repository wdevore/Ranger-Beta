#pragma once

#include "atlas.hpp"
#include "mesh.hpp"

namespace Core
{
    class VectorObject;

    class UniformAtlas final : public Atlas
    {
    private:
        void buildSquare_(VectorObject *vo);

    public:
        Mesh mesh{};

        UniformAtlas(/* args */) = default;
        ~UniformAtlas() = default;

        void construct(VectorObject *vo) override;
    };

} // namespace Core
