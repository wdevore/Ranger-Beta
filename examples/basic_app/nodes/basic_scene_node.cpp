#include <basic_scene_node.hpp>
#include <shape_generator.hpp>
#include <static_mono_atlas.hpp>

namespace Game
{
    int BasicScene::build(Core::StaticMonoAtlas &atlas)
    {
    }

    void BasicScene::receiveSignal(Core::NodeSignal signal)
    {
        std::cout << "BasicScene Got signal: '" << signal << "' id: " << id << std::endl;
    }

} // namespace GameApp
