#include <square_node.hpp>
#include <shape_generator.hpp>
#include <environment.hpp>
#include <shader.hpp>
#include <shape_generator.hpp>
#include <spaces.hpp>
#include <vector2d.hpp>

namespace Game
{
    int SquareNode::build(Core::NodeManager &nodeMan)
    {
        Core::shapeShPtr squareShape = env->atlas.getShapeByName(Core::GeneratedShapeNames::UnitRectangle);

        if (squareShape == nullptr)
        {
            Core::ShapeGenerator generator{};
            generator.generateUnitRectangle(Core::ShapeControls::Centered, Core::ShapeControls::Filled);
            Core::Shape &shape = generator.shape;
            rectangleAtlasId_ = shape.id;
            env->atlas.addShapeAndShake(shape);

            generator.generateABBox(bounds);
        }
        else
        {
            rectangleAtlasId_ = squareShape->id;
        }

        setPosition(env->deviceWidth / 2, env->deviceHeight / 2);
        // setRotation(45.0f * Core::DEGREES_TO_RADIANS);

        nodeMan.registerForTimingUpdates(shared_from_this());
        nodeMan.registerForEvents(shared_from_this());

        dragState.initialize(shared_from_this(), env, bounds);

        return 1;
    }

    void SquareNode::update(double dt)
    {
        // Calculate the rotation amount for this frame
        // To make your rotation frame-rate independent, you should not directly
        // multiply the current angle by deltaTime. Instead, you multiply the
        // desired change in angle per second (angular velocity) by deltaTime to
        // determine the actual change in angle for the current frame.
        const float rotationThisFrame = angularVelocityRadiansPerSecond * dt;

        setRotation(angle);

        // Apply the rotation to your object's angle
        angle += rotationThisFrame;

        if (angle > M_PI)
        {
            angle = M_PI - angle;
        }
        else if (angle < 0)
        {
            angle = M_PI + angle;
        }
    }

    void SquareNode::receiveSignal(Core::NodeSignal signal)
    {
        std::cout << "BasicScene Got signal: '" << signal << "' id: " << id << std::endl;
    }

    void SquareNode::render(const Core::Matrix4 &model)
    {
        env->atlas.use();

        if (dragState.pointInside)
            env->atlas.setColor({1.0, 0.75, 0.75, 1.0});
        else
            env->atlas.setColor({1.0, 0.5, 0.5, 1.0});

        env->atlas.render(rectangleAtlasId_, model);

        env->atlas.unUse();
    }

    bool SquareNode::handleEvent(const Core::IOEvent &event)
    {
        dragState.handleEvent(event);

        return true;
    }

} // namespace Game
