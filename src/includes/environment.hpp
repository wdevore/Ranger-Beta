#pragma once

#include <iostream>
#include <memory>

#include <static_mono_atlas.hpp>

namespace Core
{
    class Environment : public std::enable_shared_from_this<Environment>
    {
    private:
        /* data */
    public:
        int deviceWidth{};
        int deviceHeight{};

        const std::string rootPath{"/home/iposthuman/Development/cpp/Ranger-Beta"};
        const std::string examplePath{"/examples/basic_app"};
        const std::string shadersPath{"/examples/common/assets/shaders/"};

        StaticMonoAtlas atlas{};

        Environment(/* args */) { std::cout << "Environment ctor" << std::endl; };
        ~Environment() = default;

        void initialize();
    };

    using environmentShPtr = std::shared_ptr<Environment>;

} // namespace Core
