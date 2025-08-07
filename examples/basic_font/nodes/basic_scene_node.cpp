#include <memory>

#include <basic_scene_node.hpp>
#include <static_mono_atlas.hpp>
#include <environment.hpp>
#include <square_node.hpp>

namespace Game
{
    int BasicScene::build(Core::NodeManager &nodeMan)
    {
        zoomNode = std::make_shared<Core::ZoomNode>("Zoom", env, shared_from_this());
        zoomNode->build(nodeMan);
        appendChild(zoomNode);

        auto squareNode = std::make_shared<SquareNode>("SquareNode", env, zoomNode);
        squareNode->build(nodeMan);
        squareNode->setScale(env->deviceHeight / 4, env->deviceHeight / 4);
        zoomNode->appendChild(squareNode);

        // nodeMan.registerForEvents(shared_from_this());
        nodeMan.registerForTimingUpdates(shared_from_this());

        return 1;
    }

    void BasicScene::update(double dt)
    {
        using ioKey = Core::IOEvent::Key;
        if (env->isKeyPressed(ioKey::A) && env->isKeyPressed(ioKey::B))
        {
            std::cout << "AB" << std::endl;
        }
        else if (env->isKeyPressed(ioKey::A))
        {
            std::cout << "A" << std::endl;
        }
        else if (env->isKeyPressed(ioKey::B))
        {
            std::cout << "B" << std::endl;
        }
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
        // std::cout << "BasicScene::handleEvent " << event.x << "," << event.y << " : " << static_cast<int>(event.type) << std::endl;
        // if (env->isKeyPressed(event.key))
        // {
        //     std::cout << event.key << std::endl;
        // }
        // if (event.type == Core::IOEvent::Type::Key)
        // {
        //     std::cout << event.key << std::endl;
        // }
        return true;
    }
} // namespace GameApp
