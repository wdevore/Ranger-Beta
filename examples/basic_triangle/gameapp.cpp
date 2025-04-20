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
        return 0;
    }

} // namespace Game
