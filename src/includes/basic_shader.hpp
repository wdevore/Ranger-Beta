#pragma once

#include <shader.hpp>

namespace Core
{
    class BasicShader final : public Shader
    {
    private:
        /* data */
    public:
        BasicShader();
        explicit BasicShader(const std::string &vertexPath, const std::string &fragmentPath) : Shader(vertexPath, fragmentPath) {};
        ~BasicShader() = default;

        ErrorConditions build();
    };

} // namespace Core
