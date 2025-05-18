#include <sstream> // For std::ostringstream

#include "basic_shader.hpp"
#include "constants.hpp"

namespace Core
{
    // "../common/assets/shaders/basic_mvp.vs", "../common/assets/shaders/basic.frag"
    // BasicShader::BasicShader()
    // {
    // std::cout << "BasicShader::BasicShader()" << std::endl;

    // std::ostringstream oss;
    // oss << rootPath << shadersPath << "basic.vs";
    // vertexPath_ = oss.str();

    // oss.str("");
    // oss.clear();

    // oss << rootPath << shadersPath << "basic.frag";
    // fragmentPath_ = oss.str();
    // }

    ErrorConditions BasicShader::build()
    {
        std::cout << "BasicShader::build" << std::endl;

        return Shader::build();
    }

} // namespace Core
