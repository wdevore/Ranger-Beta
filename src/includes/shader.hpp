#pragma once

#include <iostream>

#include <glad/gl.h>
// We don't need to include opengl <gl.h> because glfw does it already.
#include <GLFW/glfw3.h>

namespace Core
{
    class Shader
    {
    private:
        /* data */
        void read();
        void compile();
        void checkCompileErrors(GLuint shader, const std::string &type);

    protected:
        GLuint program_;

        std::string vertexCode_;
        std::string fragmentCode_;

        std::string vertexPath_;
        std::string fragmentPath_;

        void build();

    public:
        Shader(const std::string &vertexPath, const std::string &fragmentPath) : vertexPath_(vertexPath), fragmentPath_(fragmentPath) {};
        ~Shader() = default;

        // Set by inherited class.
        GLuint program() const { return program_; };

        // Uses the current shader
        void use();
    };

} // namespace Core
