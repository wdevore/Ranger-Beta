#include <basic_scene_node.hpp>
#include <shape_generator.hpp>
#include <static_mono_atlas.hpp>
#include <environment.hpp>

namespace Game
{
    int BasicScene::build(Core::NodeManager &nodeMan)
    {
        Core::ShapeGenerator generator{};
        generator.generateUnitRectangle(Core::ShapeControls::Centered, Core::ShapeControls::Filled);
        Core::Shape &shape = generator.shape;
        rectangleAtlasId_ = shape.id;

        env->atlas.addShape(shape.name, shape.vertices, shape.indices, shape.primitiveMode);

        // The rectangle should be the size of the device.
        model.setScale(env->deviceWidth, env->deviceHeight);

        nodeMan.registerTarget(shared_from_this());

        return 1;
    }

    // TODO ADD UPDATE!!!!!!!
    // model.setScale(100.0, 100.0, 1.0);

    void BasicScene::receiveSignal(Core::NodeSignal signal)
    {
        std::cout << "BasicScene Got signal: '" << signal << "' id: " << id << std::endl;
    }

    void BasicScene::render(const Core::Matrix4 &model)
    {
        env->atlas.setColor({1.0, 0.5, 0.5, 1.0});

        env->atlas.render(rectangleAtlasId_, this->model);
    }

} // namespace GameApp
