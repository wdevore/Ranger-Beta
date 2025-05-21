#include <memory>

#include <basic_scene_node.hpp>
// #include <shape_generator.hpp>
#include <static_mono_atlas.hpp>
#include <environment.hpp>
#include <square_node.hpp>

namespace Game
{
    int BasicScene::build(Core::NodeManager &nodeMan)
    {
        auto squareNode = std::make_shared<SquareNode>("SquareNode", env, shared_from_this());
        squareNode->build(nodeMan);
        squareNode->setScale(env->deviceHeight / 4, env->deviceHeight / 4);

        appendChild(squareNode);

        // nodeMan.registerTarget(shared_from_this());
        nodeMan.registerForEvent(shared_from_this());

        return 1;
    }

    void BasicScene::receiveSignal(Core::NodeSignal signal)
    {
        std::cout << "BasicScene Got signal: '" << signal << "' id: " << id << std::endl;
    }

    void BasicScene::render(const Core::Matrix4 &model)
    {
        glClearColor(bgClearColor.r, bgClearColor.g, bgClearColor.b, bgClearColor.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    bool BasicScene::handleEvent(const Core::IOEvent &event)
    {
        std::cout << "BasicScene::handleEvent " << event.x << "," << event.y << std::endl;
        return true;
    }
} // namespace GameApp
