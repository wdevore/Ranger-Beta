#pragma once

#include <memory>

#include <glad/gl.h>

#include "ortho_projection.hpp"
#include "camera.hpp"

namespace Core
{
    class Environment;
    using environmentShPtr = std::shared_ptr<Environment>;

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

        environmentShPtr environment;

        std::string lastError{};

        bool dirty{true};

    public:
        BaseAtlas(/* args */) = default;
        ~BaseAtlas() = default;

        virtual void initialize(environmentShPtr environment);
    };

} // namespace Core
