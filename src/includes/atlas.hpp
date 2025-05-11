#pragma once

#include <memory>
#include <unordered_map>

#include "vector_shape.hpp"

namespace Core
{
    class VectorObject;

    class Atlas
    {
    private:
        /* data */

    protected:
        std::unordered_map<std::string, VectorShapeSPtr> shapes_{};

    public:
        Atlas(/* args */) = default;
        ~Atlas() = default;

        virtual void construct(VectorObject *vo) = 0;
        void addShape(const VectorShapeSPtr &shape);
    };

} // namespace Core
