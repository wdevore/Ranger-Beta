#pragma once

#include <static_mono_atlas.hpp>

namespace Core
{
    /// @brief Bitmap fonts defined as pixels.
    class BitmapFontBase
    {
    private:
        /* data */

    protected:
        float fontScale{1.0};

    public:
        BitmapFontBase(/* args */) = default;
        ~BitmapFontBase() = default;

        virtual void build(Core::StaticMonoAtlas &atlas);
    };

} // namespace Core
