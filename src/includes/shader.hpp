#pragma once

#include <iostream>
#include <sstream> // For std::ostringstream

#include <glad/gl.h>
// We don't need to include opengl <gl.h> because glfw does it already.
#include <GLFW/glfw3.h>

#include "constants.hpp"

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

        ErrorConditions build();

    public:
        Shader() = default;
        explicit Shader(const std::string &vertexPath, const std::string &fragmentPath) : vertexPath_(vertexPath), fragmentPath_(fragmentPath)
        {
            std::ostringstream oss;
            oss << rootPath << shadersPath << vertexPath_;
            std::cout << "Shader::Shader vp: " << oss.str() << std::endl;

            vertexPath_ = oss.str();

            oss.str("");
            oss.clear();

            oss << rootPath << shadersPath << fragmentPath_;
            std::cout << "Shader::Shader fp: " << oss.str() << std::endl;
            fragmentPath_ = oss.str();
        };
        ~Shader() = default;

        // Set by inherited class.
        GLuint program() const { return program_; };

        // Uses the current shader
        void use();
    };

} // namespace Core
