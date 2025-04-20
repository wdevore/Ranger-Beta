#include <iostream>

#include <gameapp.hpp>

namespace Game
{

    GameApp::~GameApp()
    {
    }

    int GameApp::construct()
    {
        std::cout << "Constructing game" << std::endl;

        // Create BootScene Node (scene)
        // Create SplashScene Node (scene)
        // Create GameScene Node (scene)
        // Create GameLayer Node as child of GameScene
        // Create SquareNode as a child of GameLayer

        return 0;
    }

} // namespace Game
