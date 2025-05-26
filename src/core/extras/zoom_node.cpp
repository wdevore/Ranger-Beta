#include <memory>

#include <zoom_node.hpp>
#include <node.hpp>
#include <spaces.hpp>
#include <node_manager.hpp>

namespace Core
{
    int ZoomNode::build(NodeManager &nodeMan)
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

        nodeMan.registerForEvents(shared_from_this());

        zoomStepSize = 0.05;

        return 1;
    }

    void ZoomNode::setStepSize(float size)
    {
        zoomStepSize = size;
    }

    void ZoomNode::setPostion(float x, float y)
    {
        zoom.setPosition(x, y);
        rippleDirty(true);
    }

    void ZoomNode::setFocalPoint(float x, float y)
    {
        zoom.setAt(x, y);
        rippleDirty(true);
    }

    void ZoomNode::scaleTo(float scale)
    {
        zoom.setScale(scale);
        rippleDirty(true);
    }

    float ZoomNode::getZoomScale() const
    {
        return zoom.getPseudoScale();
    }

    void ZoomNode::zoomBy(float dx, float dy)
    {
        zoom.zoomBy(dx, dy);
        rippleDirty(true);
    }

    void ZoomNode::translateBy(float dx, float dy)
    {
        zoom.translateBy(dx, dy);
        rippleDirty(true);
    }

    void ZoomNode::zoomIn()
    {
        zoom.zoomBy(zoomStepSize, zoomStepSize);
        rippleDirty(true);
    }

    void ZoomNode::zoomOut()
    {
        zoom.zoomBy(-zoomStepSize, -zoomStepSize);
        rippleDirty(true);
    }

    AffineTransform &ZoomNode::calcTransform()
    {
        if (dirty)
            dirty = false;

        return zoom.getTransform();
    }

    bool ZoomNode::handleEvent(const IOEvent &event)
    {
        if (event.type == IOEvent::Type::Moved)
        {
            // This gets the local-space coords of the rectangle node.
            if (!parent.expired())
            {
                std::shared_ptr<Node> lockedPtr = parent.lock();

                if (lockedPtr)
                {
                    mapDeviceToNode(*env, event.x, event.y, shared_from_this(), zoomPoint);

                    setFocalPoint(zoomPoint.x, zoomPoint.y);
                }
            }
        }
        else if (event.type == IOEvent::Type::Scrolled)
        {
            int dir = event.y;
            if (dir == 1)
            {
                zoomIn();
            }
            else
            {
                zoomOut();
            }
        }

        return false;
    }

} // namespace Core
