#include <basic_scene_node.hpp>

namespace Game
{
    void BasicScene::receiveSignal(Core::NodeSignal signal)
    {
        std::cout << "BasicScene Got signal: '" << signal << "' id: " << id << std::endl;
    }

} // namespace GameApp
