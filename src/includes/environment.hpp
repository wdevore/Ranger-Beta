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

        std::string rootPath{};
        std::string examplePath{};
        std::string shadersPath{};

        StaticMonoAtlas atlas{};

        Environment(/* args */) { std::cout << "Environment ctor" << std::endl; };
        ~Environment() = default;

        void initialize(int deviceWidth, int deviceHeight);
        void postInitialize();
    };

    using environmentShPtr = std::shared_ptr<Environment>;

} // namespace Core
