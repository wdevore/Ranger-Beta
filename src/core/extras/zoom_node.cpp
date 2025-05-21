#include <memory>

#include <zoom_node.hpp>
#include <node.hpp>

namespace Core
{
    int ZoomNode::build(Core::NodeManager &nodeMan)
    {
        if (!parent.expired())
        {
            // Attempt to access the object using lock()
            std::shared_ptr<Node> lockedPtr = parent.lock();
            if (lockedPtr)
            {
                lockedPtr->appendChild(shared_from_this());
            }
        }

        zoomStepSize = 0.1;

        return 1;
    }

    void ZoomNode::setStepSize(float size)
    {
        zoomStepSize = size;
    }

    void ZoomNode::setPostion(float x, float y)
    {
        zoom.setPosition(x, y);
        // TODO ripple dirty
    }

    void ZoomNode::setFocalPoint(float x, float y)
    {
        zoom.setAt(x, y);
        // ripple dirty
    }

    void ZoomNode::scaleTo(float scale)
    {
        zoom.setScale(scale);
        // ripple dirty
    }

    float ZoomNode::getZoomScale() const
    {
        return zoom.getPseudoScale();
    }

    void ZoomNode::zoomBy(float dx, float dy)
    {
        zoom.zoomBy(dx, dy);
        // ripple dirty
    }

    void ZoomNode::translateBy(float dx, float dy)
    {
        zoom.translateBy(dx, dy);
        // ripple dirty
    }

    void ZoomNode::zoomIn()
    {
        zoom.zoomBy(zoomStepSize, zoomStepSize);
        // ripple dirty
    }

    void ZoomNode::zoomOut()
    {
        zoom.zoomBy(-zoomStepSize, -zoomStepSize);
        // ripple dirty
    }

    AffineTransform &ZoomNode::calcTransform()
    {
        dirty = false;

        return zoom.getTransform();
    }

    bool ZoomNode::handleEvent(const IOEvent &event)
    {
        // TODO add mouse and space mapping.
        return false;
    }

} // namespace Core
