#pragma once

#include <glad/gl.h>

#include <iostream>
#include <iomanip>

namespace Game
{
    int checkGLError(const std::string &funcName);
} // namespace Game