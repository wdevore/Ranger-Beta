#pragma once

#include <glad/gl.h>

#include "ortho_projection.hpp"
// #include "matrix4.hpp"
#include "camera.hpp"

namespace Core
{
    class BaseAtlas
    {
    private:
        /* data */

    protected:
        const GLuint UnBindID{0};
        const GLsizei Uniform4vColorCompCount{1};
        const GLintptr GLSubDataOffset{0};
        /// @brief Format is: xyzxyzxyz...
        const int VertexStride{3}; // xyz
        const GLsizei GLUniformMatrixCount{1};
        const GLboolean GLUniformMatrixTransposed{false};

        /// @brief This sets the project and positions the Origin.
        OrthoProjection projection{};
        /// @brief This is the camera
        Camera camera{};
        // Matrix4 viewspace{true};

        std::string lastError{};

        bool dirty{true};

    public:
        BaseAtlas(/* args */) = default;
        ~BaseAtlas() = default;

        void initialize(int width, int height);
    };

} // namespace Core
