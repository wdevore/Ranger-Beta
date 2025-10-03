#include <memory>

#include <basic_scene_node.hpp>
#include <static_mono_atlas.hpp>
#include <environment.hpp>

namespace Game
{
    int BasicScene::build(Core::NodeManager &nodeMan)
    {
        return 1;
    }

    void BasicScene::update(double dt)
    {
    }

    void BasicScene::receiveSignal(Core::NodeSignal signal)
    {
    }

    void BasicScene::render(const Core::Matrix4 &model)
    {
        // glClearColor(bgClearColor.r, bgClearColor.g, bgClearColor.b, bgClearColor.a);
        // Core::checkGLError(name + "::render:glClearColor");
        // glClear(GL_COLOR_BUFFER_BIT);
        // Core::checkGLError(name + "::render:glClear");
    }

    bool BasicScene::handleEvent(const Core::IOEvent &event)
    {
        return true;
    }
} // namespace GameApp
