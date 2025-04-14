#pragma once

#include <stack>
#include <memory>

#include "matrix4.hpp"
#include "affinetransform.hpp"

namespace Core
{
    using matrixShPtr = std::shared_ptr<Matrix4>;

    class TransformStack
    {
    private:
        /* data */
    public:
        // std::stack<matrixShPtr> stack{};
        // matrixShPtr current = std::make_shared<Matrix4>(true);
        std::stack<Matrix4> stack{};
        Matrix4 current{true};
        Matrix4 post_{true}; // Pre allocated cache
        Matrix4 m4_{true};

        TransformStack(/* args */) = default;
        ~TransformStack() = default;

        void initialize(const Matrix4 &m);
        bool empty();
        // matrixShPtr applyTransform(const AffineTransform &aft);
        void applyTransform(const AffineTransform &aft);
        void save();
        void restore();
    };

} // namespace Core
