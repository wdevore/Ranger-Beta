#pragma once

#include <glad/gl.h>
// We don't need to include opengl <gl.h> because glfw does it already.
#include <GLFW/glfw3.h>

namespace Core
{
    class Mesh;

    class Ebo final
    {
    private:
        /* data */
        // Indicate if an Id has been generated yet.
        bool genBound_ = false;

        GLuint eboId_;

    public:
        Ebo(/* args */) = default;
        ~Ebo();

        void bind(Mesh *mesh);
        void gen();
        // void use();
        // void unUse();

        //! Use this more often than the draw methods.
        // void render(const VectorShapeSPtr& shape);
    };

} // namespace Core
