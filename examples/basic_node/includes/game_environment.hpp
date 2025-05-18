#pragma once

#include <environment.hpp>

namespace Game
{
    class GameEnvironment : public Core::Environment
    {
    private:
        /* data */
    public:
        GameEnvironment(/* args */)
        {
            rootPath = "/home/iposthuman/Development/cpp/Ranger-Beta";
            examplePath = "/examples/basic_app";
            shadersPath = "/examples/common/assets/shaders/";
        };
        ~GameEnvironment() = default;
    };

    using gameEnvironmentShPtr = std::shared_ptr<GameEnvironment>;

} // namespace Game
