#pragma once

#include <app.hpp>

namespace Game
{
    class GameApp : public App
    {
    private:
        /* data */
    public:
        GameApp(unsigned int width, unsigned int height) : App(width, height) {};
        ~GameApp();

        int construct() override;
    };

} // namespace Game
