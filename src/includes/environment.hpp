#pragma once

#include <iostream>
#include <memory>
#include <unordered_set>

#include <static_mono_atlas.hpp>
#include <io_event.hpp>

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

        std::unordered_set<int> pressedKeys;

        Environment(/* args */) { std::cout << "Environment ctor" << std::endl; };
        ~Environment() = default;

        void initialize(int deviceWidth, int deviceHeight);
        void postInitialize();
        void dispose();

        // -----------------------------------------------------------
        // Events
        // -----------------------------------------------------------
        /// @brief Registers a key as being pressed.
        /// @param key The GLFW key code.
        void registerKeyPressed(int key)
        {
            pressedKeys.insert(key);
        }

        /// @brief Registers a key as being released.
        /// @param key The GLFW key code.
        void registerKeyReleased(int key)
        {
            pressedKeys.erase(key);
        }

        /// @brief Checks if a specific key is currently pressed.
        /// @param key The GLFW key code.
        /// @return True if the key is pressed, false otherwise.
        bool isKeyPressed(int key) const
        {
            return pressedKeys.count(key) > 0;
        }

        bool isKeyPressed(IOEvent::Key key) const
        {
            return pressedKeys.count(static_cast<int>(key)) > 0;
        }
    };

    using environmentShPtr = std::shared_ptr<Environment>;

} // namespace Core
