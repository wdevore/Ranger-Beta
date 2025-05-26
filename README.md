# Ranger-Beta
A c++17 continuation of Ranger-Alpha and FlutterRanger

This project was started in April 2025 and is a mixture of:

- [Ranger-Alpha](https://github.com/wdevore/Ranger-Alpha) was done in March 2016
- [Ranger-Go-IDE](https://github.com/wdevore/Ranger-Go-IGE) was done is 2020
- [SimpleOpenGL-Go](https://github.com/wdevore/SimpleOpenGL-Go) was done in 2020

# Checkmark ✔

# Tasks
* Complete shapes ✔
* Event system ✔
* Integrate Node system ✔
  * Zoom Node ✔
  * Dragging ✔
  * Space mapping ✔
* Zones
* Interpolation
* Fonts
  * Vector <-- **WORKING**
  * Bitmap
* Box2D
* Particles
* Filters
* Animation (tweening) *Using tanema's library*: https://github.com/tanema/gween

# Code documentation

## Shader
A Shader has a program associated with it. You construct a Shader using Vertex and Fragment shader programs.

# Atlas
An Atlas has a Shader associated with it. It contains vector shapes.

# Junk
        // Lock on to the drag
        // if (pointInside && event.isDragStart)
        // {
        //     isDragLocked = true;
        // }
        // else if (event.button == Core::IOEvent::Button::Left && event.action == Core::IOEvent::Action::Release)
        // {
        //     isDragLocked = false;
        // }

        // On initial press 
        // we need a button down AND a drag!!!!!!!!!!!!!
        // if (!parent.expired())
        // {
        //     std::shared_ptr<Node> lockedPtr = parent.lock();
        //     if (lockedPtr)
        //     {
        //         mapDeviceToNode(*env, event.dx, event.dy, lockedPtr, mapPoint);
        //     }
        // }

        // if (active)
        // {
        //     setPosition(mapPoint.x, mapPoint.y);
        // }
        // else if (active && event.isDragging)
        // {
        //     Core::Vector2D pos = getPosition();
        //     setPosition(pos.x + mapPoint.x, pos.y + mapPoint.y);
        // }

        if (!parent.expired())
        {
            std::shared_ptr<Node> lockedPtr = parent.lock();
            if (lockedPtr)
            {
                std::cout << *this << "--- N parent of active: " << *lockedPtr << std::endl;
                mapDeviceToNode(*env, event.dx, event.dy, lockedPtr, mapPoint);
                if (isDragLocked)
                {
                    std::cout << "active: " << mapPoint << std::endl;
                    Core::Vector2D pos = getPosition();
                    setPosition(pos.x + mapPoint.x, pos.y + mapPoint.y);
                }
                else
                {
                    // std::cout << "not active: " << mapPoint << debug++ << std::endl;
                }
            }
        }
        std::cout << "END SquareNode::handleEvent ---------------------------" << std::endl;

        // if (event.isDragDown)
        // {
        //     if (pointInside)
        //     {
        //         std::cout << "square collide " << event.isDragDown << std::endl;
        //         dragState.setButton(event.x, event.y,
        //                             *env,
        //                             Core::IOEvent::Button::Left, Core::IOEvent::Action::Press,
        //                             shared_from_this());

        //         return true;
        //     }
        // }

        // if (event.isDragging && pointInside)
        // {
        //     std::cout << "dragging and inside" << std::endl;

        //     dragState.setMotion(event.x, event.y, *env, shared_from_this());

        //     Core::Vector2D position = getPosition();
        //     setPosition(position.x + dragState.delta.x, position.y + dragState.delta.y);
        // }