#pragma once

#include "atlas.hpp"

namespace Core
{
    class UniformAtlas final : public Atlas
    {
    private:
        void buildSquare_(VectorObject &vo);

    public:
        UniformAtlas(/* args */) = default;
        ~UniformAtlas() = default;

        void construct(VectorObject &vo) override;
    };

} // namespace Core
