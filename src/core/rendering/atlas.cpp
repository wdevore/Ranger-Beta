#include "atlas.hpp"

namespace Core
{

    Atlas::Atlas(/* args */)
    {
        name = "Atlas";
    }

    Atlas::~Atlas()
    {
    }

    GLint Atlas::fetchUniformVar(GLuint program, const std::string &uniName)
    {
        GLint location = glGetUniformLocation(program, uniName.c_str());
        int err = Core::checkGLError("::fetchUniforms:glGetUniformLocation uni: " + uniName);
        if (err < 0)
        {
            return -1;
        }
        if (location < 0)
        {
            std::cout << "Fetch Uniform Error: " << "Couldn't find '" + uniName + "' uniform variable" << std::endl;
            return -1;
        }
        std::cout << name << "::fetchUniforms: location for '" << uniName << "': " << location << std::endl;
        return location;
    }

    ErrorConditions Atlas::configureUniforms()
    {
        shader.use();

        modelLoc = fetchUniformVar(shader.program(), uniModel);
        if (modelLoc < 0)
            return Core::ErrorConditions::GLUniformVarNotFound;

        return Core::ErrorConditions::None;
    }

    // This is the same as "baking" the shapes. Copies backing data to GPU.
    int Atlas::transferToGPU()
    {
        // Start bake sequence
        startBind();

        bind();

        completeBind();

        return 0;
    }

    int Atlas::startBind()
    {
        // ---------------------------------------------------------
        // BEGIN VAO Scope and generate buffer ids
        // ---------------------------------------------------------
        glGenVertexArrays(1, &vaoID);
        checkGLError(name + ": Atlas::startBind Gen VAO");

        // This VAO bind starts the VAO Scope
        // Bind the Vertex Array Object first, then bind and set vertex buffer(s)
        // and attribute pointer(s).
        glBindVertexArray(vaoID); // Specifies the name of the vertex array to bind.
        checkGLError(name + ": Atlas::startBind bind VAO");

        glGenBuffers(1, &vboID);
        checkGLError(name + ": Atlas::startBind Gen VBO");

        if (usesEbo)
        {
            glGenBuffers(1, &eboID);
            checkGLError(name + ": Atlas::startBind Gen EBO");
        }

        return 0;
    }

    ErrorConditions Atlas::bind()
    {
        // The total buffer sizes are count of types (i.e floats or ints) times
        // the size of the type. Thus the size is in Bytes
        int vboBufferSize = vertices.size() * sizeof(GLfloat);
        // int vboBufferSize = backingShape.vertices.size() * Core::XYZComponentCount * sizeof(GLfloat); // Needlessly bigger

        if (usesEbo)
        {
            int eboBufferSize = indices.size() * sizeof(GLuint);
            if (vboBufferSize == 0 || eboBufferSize == 0)
            {
                return ErrorConditions::GLBuffersSize;
            }
            eboBind(eboBufferSize, indices);
            Core::checkGLError("ShapesAtlas::bind eboBind");
        }
        else
        {
            if (vboBufferSize == 0)
                return ErrorConditions::GLBuffersSize;
        }

        vboBind(vboBufferSize, vertices);
        Core::checkGLError("ShapesAtlas::bind vboBind");

        // Count == (xyz=3) * sizeof(float32)=4 == 12 thus each
        // vertex is 12 bytes
        int vertexSize = GLuint(Core::XYZComponentCount) * sizeof(GLfloat);

        // Some shaders may have normals and/or textures coords.
        // We only have one attribute (position) in the shader, so the
        // 'position' attribute defaults to zero.
        // In the shader we gave the attrib index the name "aPos"
        const GLuint positionIndex{0}; // aPos
        const GLboolean normalized{false};

        // We can link the attribute position with the data in the vertexData array
        glVertexAttribPointer(positionIndex, GLuint(Core::XYZComponentCount), GL_FLOAT, normalized, vertexSize, (GLvoid *)(0));
        Core::checkGLError("ShapesAtlas::bind glVertexAttribPointer");

        return ErrorConditions::None;
    }

    void Atlas::vboBind(int bufferSize, const std::vector<GLfloat> &vertices)
    {
        // the buffer type of a vertex buffer object is GL_ARRAY_BUFFER
        // From this point on any buffer calls we make (on the GL_ARRAY_BUFFER target)
        // will be used to configure the currently bound buffer, which is VBO
        glBindBuffer(GL_ARRAY_BUFFER, vboID);

        // Create and fill buffer
        glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices.data(), GL_STATIC_DRAW);
    }

    void Atlas::eboBind(int bufferSize, const std::vector<GLuint> &indices)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, indices.data(), GL_STATIC_DRAW);
    }

    int Atlas::completeBind()
    {
        // ----------------------------------------------
        // END VAO Scope
        // ----------------------------------------------
        glEnableVertexAttribArray(UnBindID);
        checkGLError(name + ": Atlas::completeBind end VAO scope");

        glBindBuffer(GL_ARRAY_BUFFER, UnBindID);
        checkGLError(name + ": Atlas::completeBind glBindBuffer");
        glBindVertexArray(UnBindID);
        checkGLError(name + ": Atlas::completeBind glBindVertexArray");

        // We don't need the backing geometry resources anymore now that data
        // has been uploaded to the GPU.
        vertices.clear();
        indices.clear();

        return 0;
    }

    void Atlas::render(const Matrix4 &model)
    {
        glUniformMatrix4fv(modelLoc, GLUniformMatrixCount, GLUniformMatrixTransposed, model.data());
    }

    void Atlas::dispose()
    {
        std::cout << name << "::dispose" << std::endl;

        // Clear again just incase.
        vertices.clear();
        indices.clear();

        glDeleteVertexArrays(1, &vaoID);
        if (usesEbo)
        {

            glDeleteBuffers(1, &eboID);
        }
        glDeleteBuffers(1, &vboID);

        GLuint program = shader.program();
        glDeleteProgram(program);
    }

} // namespace Core
