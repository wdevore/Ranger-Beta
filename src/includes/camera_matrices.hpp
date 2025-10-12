#pragma once

namespace Core
{
    struct CameraMatrices
    {
        // std140: mat4 is 16 * 4 = 64 bytes.
        // It is aligned to 16 bytes, so no extra padding is needed here.

        /// @brief Projection matrix
        float p[16] = {};
        /// @brief View matrix
        float v[16] = {};
    };
} // namespace Core
