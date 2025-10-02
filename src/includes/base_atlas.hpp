#pragma once

#include <memory>

#include <glad/gl.h>

#include "ortho_projection.hpp"

namespace Core
{
    class Environment;
    using environmentShPtr = std::shared_ptr<Environment>;

    const GLuint UnBindID{0};
    const GLuint VertexAttribIndex{0};

    class BaseAtlas
    {
    private:
        /* data */

    protected:
        const GLsizei Uniform4vColorCompCount{1};
        const GLintptr GLSubDataOffset{0};
        /// @brief Format is: xyzxyzxyz...
        const int VertexStride{3}; // xyz
        /// TODO Add docs on this
        const GLsizei GLUniformMatrixCount{1};
        /// TODO Add docs on this
        const GLboolean GLUniformMatrixTransposed{false};

        /// @brief This sets the project and positions the Origin.
        OrthoProjection projection{};

        environmentShPtr environment;

        std::string lastError{};

        bool dirty{true};

        std::string name{};

    public:
        BaseAtlas(/* args */) = default;
        ~BaseAtlas() = default;

        virtual void initialize(environmentShPtr environment);
    };

} // namespace Core
