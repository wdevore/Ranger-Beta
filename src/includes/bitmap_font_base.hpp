#pragma once

#include <unordered_map>

#include <shape_generator.hpp>

namespace Core
{
    /// @brief Bitmap fonts defined as pixels.
    /// Abstract
    class BitmapFontBase
    {
    private:
        /* data */

    protected:
        float fontScale{1.0};

    public:
        BitmapFontBase(/* args */) = default;
        ~BitmapFontBase() = default;

        virtual void build() = 0;

        virtual Core::ShapeGenerator getGenerator() = 0;
        virtual std::unordered_map<char, int> getIndicesOffsets() = 0;
    };
} // namespace Core
