#include <square_node.hpp>
#include <shape_generator.hpp>
#include <environment.hpp>
#include <shader.hpp>
#include <shape_generator.hpp>

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
            env->atlas.addShape(shape.name, shape.vertices, shape.indices, shape.primitiveMode);
        }
        else
        {
            rectangleAtlasId_ = squareShape->id;
        }

        setPosition(100.0f, 100.0f);

        nodeMan.registerTarget(shared_from_this());

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
        env->atlas.setColor({1.0, 0.5, 0.5, 1.0});
        env->atlas.render(rectangleAtlasId_, model);
    }

} // namespace Game
