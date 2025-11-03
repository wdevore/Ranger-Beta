#pragma once

#include <vector>

#include <glad/gl.h>

// We don't need to include opengl <gl.h> because glfw does it already.
#include <GLFW/glfw3.h>

#include "global_data.hpp"

namespace Game
{
    class UboBuffer
    {

    private:
        const GLsizeiptr UBO_SIZE = 2 * sizeof(float[16]); // Should be 128 bytes
        GLuint uboId;

        void _linkShaderUBO(GLuint shaderProgramID, const std::string &blockName, GLuint bindingPoint);
        void _updateCameraUBO(GLuint UBO_Camera, const Core::Matrix4 &projection, const Core::Matrix4 &view);

    public:
        UboBuffer(/* args */);
        ~UboBuffer();

        /// @brief Builds UBO and binds the provided shader programs. It will
        ///        set the Projection and View uniforms.
        /// @param programIds A list of shader program IDs
        /// @param blockName Typcally "Matrices" as defined in the shader. Default to "Matrices"
        /// @param bindingPoint Typically the 0 bind point. Defaults to 0
        void associateShaderIds(
            const std::vector<GLuint> &programIds,
            const std::string &blockName = "Matrices",
            GLuint bindingPoint = 0);
        void dispose();
    };

} // namespace Game
