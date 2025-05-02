#pragma once

#include <glad/gl.h>
// We don't need to include opengl <gl.h> because glfw does it already.
#include <GLFW/glfw3.h>

namespace Core
{
    class Mesh;

    class Vao final
    {
    private:
        /* data */
        // Indicate if an Id has been generated yet.
        bool genBound_ = false;

        GLuint vaoId_;

    public:
        static constexpr const int XYZ_Component_count = 3;

        Vao(/* args */) = default;
        ~Vao();

        void construct();
        void bind(Mesh *mesh);
        void use();
        void unUse();
        void release();

        //! Use this more often than the draw methods.
        // void render(const VectorShapeSPtr& shape);

        //! Use this more often than the draw methods.
        // void render(const VectorShapeSPtr& shape);
    };

} // namespace Core
