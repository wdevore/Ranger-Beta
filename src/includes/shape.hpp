#pragma once

#include <string>
#include <vector>
#include <memory>

#include <glad/gl.h>
// We don't need to include opengl <gl.h> because glfw does it already.
#include <GLFW/glfw3.h>

namespace Core
{
    class Shape;
    using shapeShPtr = std::shared_ptr<Shape>;

    /// @brief A shape's main purpose is to hold rendering information.
    class Shape final
    {
    private:
        /* data */
    public:
        int id{0};
        std::string name{};
        bool dirty{true};

        // For static Atlases these vertices maybe discarded after construction.
        std::vector<GLfloat> vertices{};
        int vertexOffset;

        std::vector<GLuint> indices{};

        // When you are using an EBO (which is the recommended way to handle
        // indexed drawing), the last parameter should be treated as a byte offset.
        //
        // Here is a rendering call which requires three primary pieces of information:
        // glDrawElements(shape->primitiveMode, shape->indicesCount, GL_UNSIGNED_INT, shape->dataIndicesOffset());
        //
        // The main parameters are:
        // 1) how many indices make up the shape and
        // 2) the offset into the indices buffer.
        // The shape's indices offset into the EBO buffer and this field marks it.
        GLuint indicesOffset{}; // offset in Bytes
        GLsizei indicesCount{}; // EBO indices length
        GLenum primitiveMode{}; // Example: GL_TRIANGLES

        Shape(/* args */) = default;
        ~Shape() = default;

        void clear();

        // (GLvoid *)0
        GLvoid *dataIndicesOffset()
        {
            // This is a more type-safe way to handle non-zero offsets.
            // It first casts nullptr to a char* (since pointer arithmetic on
            // char* is byte-wise), adds the byte offset, and then casts the
            // result back to void*.
            return static_cast<void *>(static_cast<char *>(nullptr) + indicesOffset);
        }
    };

} // namespace Core
