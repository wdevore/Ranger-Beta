#pragma once

namespace Core
{
    struct Color4
    {
        float r{};
        float g{};
        float b{};
        float a{};

        Color4(float r, float g, float b, float a);
        // Color4(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
    };

} // namespace Core
