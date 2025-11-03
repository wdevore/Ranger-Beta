#pragma once

#include <string>

#include <glad/gl.h>
// We don't need to include opengl <gl.h> because glfw does it already.
#include <GLFW/glfw3.h>

#include "global_data.hpp"

namespace Core
{

    class Shader
    {
    private:
        /* data */
        ErrorConditions read();
        ErrorConditions compile();
        ErrorConditions checkCompileErrors(GLuint shader, const std::string &type);

    protected:
        GLuint program_;

        std::string vertexCode_;
        std::string fragmentCode_;

        std::string vertexPath_;
        std::string fragmentPath_;

    public:
        Shader() = default;
        explicit Shader(const std::string &vertexPath, const std::string &fragmentPath) : vertexPath_(vertexPath), fragmentPath_(fragmentPath) {};
        ~Shader() = default;

        void specifyShaderFiles(const std::string &vertexPath, const std::string &fragmentPath);

        void initialize();
        ErrorConditions build();

        // Set by inherited class.
        GLuint program() const { return program_; };

        // Uses the current shader
        void use();
    };

} // namespace Core
