#include "misc.hpp"

namespace Game
{
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

} // namespace Game
