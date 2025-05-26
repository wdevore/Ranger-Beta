#include <iostream>

#include <drag_state.hpp>
#include <spaces.hpp>

namespace Core
{
    void DragState::initialize(nodeShPtr node, environmentShPtr environment, const Rectangle &bounds)
    {
        this->node = node;
        this->env = environment;
        this->bounds = bounds;
    }

    bool DragState::handleEvent(const IOEvent &event)
    {
        // Node doesn't care about Scroll events, just Moved events
        if (event.type == IOEvent::Type::Scrolled)
            return false;

        // --- Mouse Enter/Exit/Hover Logic ---
        bool wasMouseOver = pointInside;

        bool isCurrentlyMouseOver = isPointInside_(event.x, event.y);

        // Update the node's persistent mouse-over state for rendering and future events
        // However, we want to keep the pointInside state as long as we are dragging
        // the node. Once dragging is locked-on we don't want to recognize
        // inside/outside state because it doesn't matter as long as we are dragging.
        // Also, it is still need by outside code.
        if (!isDragLocked)
            pointInside = isCurrentlyMouseOver;

        // std::cout
        //     << "Type: " << static_cast<int>(event.type) << ","
        //     << " Action: " << static_cast<int>(event.action) << ","
        //     << " Button: " << static_cast<int>(event.button) << ","
        //     << " isDragLocked: " << isDragLocked << ","
        //     << " global pos: (" << event.x << "," << event.y << "),"
        //     << std::endl;

        // --- Pan/Drag Logic ---
        // Pan/Drag requires a parent context for coordinate transformation of mouse input.
        if (!node->parent.expired())
        {
            std::shared_ptr<Node> lockedParent = node->parent.lock();
            if (lockedParent)
            {
                // onPanDown / Drag Start
                if (pointInside && event.isDragStart && event.button == IOEvent::Button::Left && event.action == IOEvent::Action::Press)
                {
                    isDragLocked = true;
                    panStartNodePosition_ = node->getPosition(); // Node's current position (relative to its parent)
                    // Map current global mouse coords to the parent's local coordinate space
                    mapDeviceToNode(*env, event.x, event.y, lockedParent, panStartMouseInParentSpace_);

                    // std::cout << "onPanDown on " << node
                    //           << " at Global: (" << event.x << "," << event.y << ")"
                    //           << ", MappedStartInParent: " << panStartMouseInParentSpace_
                    //           << ", NodeStartPos: " << panStartNodePosition_ << std::endl;
                }
                // onPanUpdate / Dragging
                else if (isDragLocked && event.type == IOEvent::Type::Moved)
                {
                    // Map device coords into Node's parent space.
                    mapDeviceToNode(*env, event.x, event.y, lockedParent, currentMouseInParentSpace_);

                    panDeltaInParentSpace_.set(currentMouseInParentSpace_.x - panStartMouseInParentSpace_.x,
                                               currentMouseInParentSpace_.y - panStartMouseInParentSpace_.y);

                    // std::cout << "onPanUpdate on " << node
                    //           << " - CurrentMappedInParent: " << currentMouseInParentSpace_
                    //           << ", DeltaInParent: " << panDeltaInParentSpace_
                    //           << " (Global dx,dy: " << event.dx << "," << event.dy << ")" << std::endl;

                    node->setPosition(panStartNodePosition_.x + panDeltaInParentSpace_.x,
                                      panStartNodePosition_.y + panDeltaInParentSpace_.y);
                }
            }

            // onPanEnd / Drag Release (occurs when the button is released)
            if (event.button == IOEvent::Button::Left && event.action == IOEvent::Action::Release)
            {
                // if (isDragLocked) // Only if a drag was active on this node
                // {
                //     std::cout << "onPanEnd (Release) on " << *node << std::endl;
                //     // TODO: Implement onPanEnd/onPanCancel logic
                // }
                isDragLocked = false;
            }
        }

        return true;
    }

    bool DragState::isPointInside_(double x, double y)
    {
        mapDeviceToNode(*env, x, y, node, localPosition);
        return bounds.pointInside(localPosition);
    }
} // namespace Core
