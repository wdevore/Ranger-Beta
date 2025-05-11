#pragma once

#include <cmath>
#include <iostream>

namespace Core
{
    static const double EPSILON = 0.00001;
    static const double DEGREES_TO_RADIANS = M_PI / 180.0;
    static const double RADIANS_TO_DEGREES = 180.0 / M_PI;

    static const std::string rootPath{"/home/iposthuman/Development/cpp/Ranger-Beta"};
    static const std::string examplePath{"/examples/basic_app"};
    static const std::string shadersPath{"/examples/common/assets/shaders/"};

    enum class ErrorConditions
    {
        /// @brief No Error occurred.
        None,
        /// @brief A signal was either sent or received that wasn't recognized
        UnknownSignal,
        /// @brief Attempt to remove Node from stage that wasn't at the top
        AttemptingToRemoveNonTopNode,
        /// @brief VBO/EBO buffers are zero in size
        GLBuffersSize,
        GLUniformVarNotFound,
        GLShaderFileNotFound,
        GLShaderCompileError,
        GLShaderLinkError,
        GLFunctionError,
    };

    std::ostream &operator<<(std::ostream &os, ErrorConditions s);

    /// @brief Indicates how many parts to a vertex
    static const int XYZComponentCount = 3;

    /// @brief Is a composite of 2D vertex and 2D texture coords
    static const int XYZWComponentCount = 4;

    enum class ShapeControls
    {
        Centered,
        Filled,
        Outline,
    };

    // ------------------------- OpenGL specific -----------------
    int checkGLError(const std::string &funcName);
}