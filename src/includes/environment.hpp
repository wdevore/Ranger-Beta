#pragma once

#include <iostream>
#include <memory>
#include <unordered_set>

#include <static_mono_atlas.hpp>
#include <io_event.hpp>
#include <camera.hpp>
#include <bitmap_font_atlas.hpp>
#include <vector_font_atlas.hpp>

namespace Core
{
    // using bvFontAtlasUnqPtr = std::unique_ptr<BitmapVectorFontAtlas>;

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
        BitmapVectorFontAtlas bitmapFontAtlas{};
        VectorFontAtlas vectorFontAtlas{};

        // ---------- Fonts ---------------
        // bvFontAtlasUnqPtr fontAtlas;

        std::unordered_set<int> pressedKeys;

        /// @brief This is the camera
        Camera camera{};

        Environment(/* args */) { std::cout << "Environment ctor" << std::endl; };
        ~Environment() = default;

        void initialize(int deviceWidth, int deviceHeight);
        void postInitialize();
        void dispose();

        // /// @brief Takes ownership of provided Atlas
        // /// @param fontAtlas
        // void setFontAtlas(bvFontAtlasUnqPtr fontAtlas);

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
