#pragma once

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

        virtual void build();
    };

} // namespace Core
