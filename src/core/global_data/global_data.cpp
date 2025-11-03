// #include "../core_environment.hpp"
#include <iostream>
#include <iomanip>

#include <glad/gl.h>
// We don't need to include opengl <gl.h> because glfw does it already.
#include <GLFW/glfw3.h>

#include "global_data.hpp"

namespace Core
{
    // DEFINITION AND INITIALIZATION: Memory is allocated here, and only here.
    Matrix4 viewSpace{true};
    Matrix4 intViewSpace{};
    Matrix4 projection{};

    unsigned int deviceWidth{};
    unsigned int deviceHeight{};

    const std::string rootPath = "/home/iposthuman/Development/cpp/Ranger-Beta";
    const std::string examplePath = "/examples/basic_app";
    const std::string shadersPath = "/examples/common/assets/shaders/";

    const unsigned int UnBindID{0};

    std::ostream &operator<<(std::ostream &os, ErrorConditions s)
    {
        switch (s)
        {
        case ErrorConditions::None:
            os << "None";
            break;
        case ErrorConditions::UnknownSignal:
            os << "UnknownSignal";
            break;
        case ErrorConditions::AttemptingToRemoveNonTopNode:
            os << "AttemptingToRemoveNonTopNode";
            break;
        default:
            os << "Unknown ErrorConditions";
            break;
        }
        return os;
    }

    int checkGLError(const std::string &funcName)
    {
        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            std::cerr << "OpenGL Error in '" << funcName
                      << "': 0x" << std::hex << std::uppercase << std::setw(4) << std::setfill('0')
                      << error << std::endl;

            return -1;
        }

        return 0;
    }

} // namespace Core
