#include "transform_stack.hpp"

namespace Core
{
    void TransformStack::initialize(const Matrix4 &m)
    {
        // current->set(m);
        current.set(m);
    }

    bool TransformStack::empty()
    {
        return stack.empty();
    }

    /// @brief Access 'current' after calling this method.
    /// @param aft
    void TransformStack::applyTransform(const AffineTransform &aft)
    {
        // Concat this transform onto the current transform but don't push it.
        // Use post multiply
        m4_.set(aft);
        // multiply(current.get(), m4_, post_);
        // current->set(post_);
        multiply(&current, m4_, post_);
        current.set(post_);
    }

    void TransformStack::save()
    {
        stack.push(current);
    }

    void TransformStack::restore()
    {
        // Pop top off stack. First capture it into 'current'
        auto top = stack.top();

        current.set(top);
        // current = top; // copy constructor

        // Now pop it off into the void
        stack.pop();
    }

} // namespace Core
