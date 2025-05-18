#pragma once

#include <iostream>
#include <sstream> // For std::ostringstream
#include <memory>

#include <glad/gl.h>
// We don't need to include opengl <gl.h> because glfw does it already.
#include <GLFW/glfw3.h>

#include "constants.hpp"

namespace Core
{
    class Environment;
    using environmentShPtr = std::shared_ptr<Environment>;

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

        environmentShPtr environment;

        ErrorConditions build();

    public:
        Shader() = default;
        explicit Shader(const std::string &vertexPath, const std::string &fragmentPath) : vertexPath_(vertexPath), fragmentPath_(fragmentPath) {};
        ~Shader() = default;

        void initialize(environmentShPtr environment);

        // Set by inherited class.
        GLuint program() const { return program_; };

        // Uses the current shader
        void use();
    };

} // namespace Core
