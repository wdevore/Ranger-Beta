#include "ubo_buffer.hpp"

namespace Game
{
    UboBuffer::UboBuffer(/* args */)
    {
    }

    UboBuffer::~UboBuffer()
    {
    }

    void UboBuffer::associateShaderIds(const std::vector<GLuint> &programIds, const std::string &blockName, GLuint bindingPoint)
    {
        glGenBuffers(1, &uboId);

        // 2. Bind the UBO to GL_UNIFORM_BUFFER target
        glBindBuffer(GL_UNIFORM_BUFFER, uboId);
        Core::checkGLError("UboBuffer::configure : glBindBuffer");

        // 3. Allocate the memory for the UBO on the GPU
        // We allocate 128 bytes (2 * mat4) but don't upload data yet.
        glBufferData(GL_UNIFORM_BUFFER, UBO_SIZE, nullptr, GL_STATIC_DRAW);
        Core::checkGLError("UboBuffer::configure : glBufferData");

        // 4. Bind the UBO to a specific Uniform Buffer Binding Point (e.g., binding point 0)
        // This is the core step that links the UBO to the GLSL block's 'binding = 0'.
        glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, uboId);
        Core::checkGLError("UboBuffer::configure : glBindBufferBase");

        // 5. Unbind the UBO. We don't need to bind again because we are not
        //    going to change anything while the app runs.
        glBindBuffer(GL_UNIFORM_BUFFER, Core::UnBindID);

        // 6. linking shader programs
        for (GLuint programId : programIds)
        {
            _linkShaderUBO(programId, blockName, bindingPoint);
            Core::checkGLError("UboBuffer::configure : _linkShaderUBO");
        }

        // Note: make sure projection and viewSpace are set prior
        // 7. Just need to do this once
        _updateCameraUBO(uboId, Core::projection, Core::viewSpace);
    }

    void UboBuffer::dispose()
    {
        glDeleteBuffers(1, &uboId);
        Core::checkGLError("glDeleteBuffers UBO_Camera");
    }

    void UboBuffer::_linkShaderUBO(GLuint shaderProgramID, const std::string &blockName, GLuint bindingPoint)
    {
        // 1. Get the index of the Uniform Block within the shader program
        GLuint uniformBlockIndex = glGetUniformBlockIndex(shaderProgramID, blockName.c_str());
        Core::checkGLError("UboBuffer::_linkShaderUBO : " + std::string(blockName));

        if (uniformBlockIndex == GL_INVALID_INDEX)
        {
            std::cerr << "Error: Could not find UBO block '" << blockName
                      << "' in shader program " << shaderProgramID << std::endl;
            return;
        }

        // 2. Link the Uniform Block Index to the desired Binding Point
        glUniformBlockBinding(shaderProgramID, uniformBlockIndex, bindingPoint);
        Core::checkGLError("UboBuffer::_linkShaderUBO : " + std::string(blockName));
    }

    void UboBuffer::_updateCameraUBO(GLuint uboId, const Core::Matrix4 &projection, const Core::Matrix4 &view)
    {
        // std::cout << "UboBuffer::_updateCameraUBO projection: " << projection << std::endl;
        // std::cout << "UboBuffer::_updateCameraUBO view: " << view << std::endl;

        // Bind the UBO
        glBindBuffer(GL_UNIFORM_BUFFER, uboId);

        // 1. Upload Projection Matrix (Offset 0 bytes)
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(projection.e), projection.data());
        Core::checkGLError("UboBuffer::glBufferSubData proj");

        // 2. Upload View Matrix (Offset 64 bytes)
        // The offset for the second mat4 is exactly 64 bytes (1 mat4 * 64 bytes/mat4)
        size_t viewOffset = sizeof(projection.e);
        glBufferSubData(GL_UNIFORM_BUFFER, viewOffset, sizeof(view.e), view.data());
        Core::checkGLError("UboBuffer::glBufferSubData view");

        // Unbind the UBO
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

} // namespace Game
