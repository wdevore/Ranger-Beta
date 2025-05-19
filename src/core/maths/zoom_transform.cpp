#include <zoom_transform.hpp>

namespace Core
{

    void ZoomTransform::update()
    {
        // Accumulate zoom transformations.
        // acc_transform is an intermediate accumulative matrix used for tracking the current zoom target.
        accTransform.translate(zoomAt.x, zoomAt.y);
        accTransform.scale(scale.x, scale.y);
        accTransform.translate(-zoomAt.x, -zoomAt.y);

        // We reset Scale because acc_transform is accumulative and has "captured" the information.
        scale.set(1.0, 1.0);

        // We want to leave acc_transform solely responsible for zooming.
        // "transform" is the final matrix.
        transform.set(accTransform);

        // Tack on translation. Note: we don't append it, but concat it into a separate matrix.
        transform.translate(position.x, position.y);
    }

    void ZoomTransform::setPosition(float x, float y)
    {
        position.set(x, y);
    }

    void ZoomTransform::zoomBy(float dx, float dy)
    {
        scale.add(dx, dy);
    }

    void ZoomTransform::translateBy(float dx, float dy)
    {
        position.add(dx, dy);
    }

    float ZoomTransform::getScale() const
    {
        return scale.x;
    }

    float ZoomTransform::getPseudoScale() const
    {
        return accTransform.getPsuedoScale();
    }

    void ZoomTransform::setScale(float scale)
    {
        update();

        // We use dimensional analysis to set the scale. Remember we can't
        // just set the scale absolutely because acc_transform is an accumulating matrix.
        // We have to take its current value and compute a new value based
        // on the passed in value.

        // Also, I can use acc_transform.a because I don't allow rotations for zooms,
        // so the diagonal components correctly represent the matrix's current scale.
        // And because I only perform uniform scaling I can safely use just the "a" element.
        float scaleFactor = scale / accTransform.getPsuedoScale();

        this->scale.set(scaleFactor, scaleFactor);
    }

    void ZoomTransform::setAt(float x, float y)
    {
        zoomAt.set(x, y);
    }

    AffineTransform &ZoomTransform::getTransform()
    {
        update();
        return transform;
    }
} // namespace Core
