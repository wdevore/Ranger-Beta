#pragma once

#include <glad/gl.h>
// We don't need to include opengl <gl.h> because glfw does it already.
#include <GLFW/glfw3.h>

#include <vector>

#include "shader.hpp"

namespace Core
{
    class Atlas
    {
    private:
        int nextShapeID_{};

    protected:
        // For static Atlases these Geometry elements maybe discarded after
        // data is move to the GPU. These are backing buffers that are
        // filled by a Shape's geometry data.
        // The buffers are then copy to the GPU.
        std::vector<GLfloat> vertices{};
        std::vector<GLuint> indices{};

        /// TODO Add docs on this
        const GLsizei GLUniformMatrixCount{1};
        /// TODO Add docs on this
        const GLboolean GLUniformMatrixTransposed{false};

        // Buffer IDs
        GLuint vaoID{};
        GLuint vboID{};
        GLuint eboID{};
        bool usesEbo{true};

        // Call the setter method to set shader file names.
        Shader shader{};

        GLint modelLoc{};
        const std::string uniModel{"model"}; // \x00

        // All these method constitute "baking"
        int startBind();
        ErrorConditions bind();
        int completeBind();

        void vboBind(int bufferSize, const std::vector<GLfloat> &vertices);
        void eboBind(int bufferSize, const std::vector<GLuint> &indices);

        /// @brief Call transferToGPU when you are done adding shapes.
        /// @return
        int transferToGPU();

    public:
        std::string name{};

        Atlas(/* args */);
        ~Atlas();

        inline void specifyShaderFiles(const std::string &vertexPath, const std::string &fragmentPath)
        {
            shader.specifyShaderFiles(vertexPath, fragmentPath);
        }

        inline void begin()
        {
            glBindVertexArray(vaoID);
            checkGLError(name + "::begin");
        }

        inline void use()
        {
            glBindVertexArray(vaoID);
            checkGLError("Atlas::use glBindVertexArray vaoID");

            // See opengl wiki as to why "glBindVertexArray(0)" isn't really necessary here:
            // https://www.opengl.org/wiki/Vertex_Specification#Vertex_Buffer_Object
            // Note the line "Changing the GL_ARRAY_BUFFER binding changes nothing about vertex attribute 0..."
            shader.use();
            checkGLError("Atlas::use shader use");

            glEnable(GL_PROGRAM_POINT_SIZE);
        }
        inline void unUse()
        {
            glDisable(GL_PROGRAM_POINT_SIZE);

            glBindVertexArray(UnBindID);
            checkGLError("Atlas::unUse glBindVertexArray UnBindID");
        }

        virtual void dispose();

        /// @brief Use the method to pass shader program ids to a UBO for binding.
        /// @return Shader's program id.
        inline GLuint program() const { return shader.program(); };

        GLint fetchUniformVar(GLuint program, const std::string &uniName);
        virtual ErrorConditions configureUniforms();

        /// @brief This is used by special nodes that don't render anything but instead
        /// manipulate the node, for example, ZoomNode.
        /// @param model
        void render(const Matrix4 &model);
    };

} // namespace Core
