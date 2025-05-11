#include <memory>    // std::make_shared
#include <algorithm> // For std::find or std::find_if

#include <static_mono_atlas.hpp>
#include <constants.hpp>

namespace Core
{

    ErrorConditions StaticMonoAtlas::configure()
    {
        // Load shader pograms
        ErrorConditions buildStatus = shader.build();
        if (buildStatus != ErrorConditions::None)
            return buildStatus;

        return ErrorConditions::None;
    }

    ErrorConditions StaticMonoAtlas::shake()
    {
        // ---------------------------------------------------------
        // Collect all vertices and indices for buffers
        // ---------------------------------------------------------
        // The atlas has shapes and each shape has vertices. These need to be
        // combined into a single array and later copied into GL Buffer.
        // At the same time each shape needs to be updated
        // to adjust element offsets and counts.
        int indicesOffset = 0;
        int indiceBlockOffset = 0;

        for (auto &&shape : shapes)
        {
            shape->indicesOffset = indicesOffset;

            indicesOffset += shape->indices.size() * sizeof(GLuint); // bytes

            // Backing store
            for (auto &&vertex : shape->vertices)
                backingShape.vertices.push_back(vertex);

            // The indice offset is always refering to a position within
            // the vertices array.
            for (auto &&i : shape->indices)
                backingShape.indices.push_back(GLuint(i + indiceBlockOffset));

            // Offset the indices based on the vertice block position.
            indiceBlockOffset = GLuint(backingShape.vertices.size() / Core::XYZComponentCount);
        }

        return ErrorConditions::None;
    }

    ErrorConditions StaticMonoAtlas::bake()
    {
        // ---------------------------------------------------------
        // BEGIN VAO Scope and generate buffer ids
        // ---------------------------------------------------------
        glGenVertexArrays(1, &vaoID);

        // This VAO bind starts the VAO Scope
        // Bind the Vertex Array Object first, then bind and set vertex buffer(s)
        // and attribute pointer(s).
        glBindVertexArray(vaoID); // Specifies the name of the vertex array to bind.

        glGenBuffers(1, &vboID);

        glGenBuffers(1, &eboID);

        // The total buffer sizes are count of types (i.e floats or ints) times
        // the size of the type. Thus the size is in Bytes
        int vboBufferSize = backingShape.vertices.size() * Core::XYZComponentCount * sizeof(GLfloat);
        int eboBufferSize = backingShape.indices.size() * sizeof(GLuint);

        if (vboBufferSize == 0 || eboBufferSize == 0)
        {
            return ErrorConditions::GLBuffersSize;
        }

        vboBind(vboBufferSize, backingShape.vertices);

        eboBind(vboBufferSize, backingShape.indices);

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

        // ----------------------------------------------
        // END VAO Scope
        // ----------------------------------------------
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        ErrorConditions configureUniStatus = configureUniforms();

        if (configureUniStatus != ErrorConditions::None)
            return configureUniStatus;

        backingShape.vertices.clear();
        backingShape.indices.clear();

        return ErrorConditions::None;
    }

    ErrorConditions StaticMonoAtlas::configureUniforms()
    {
        std::cout << "StaticMonoAtlas::configureUniforms" << std::endl;

        shader.use();

        GLuint program = shader.program();

        modelLoc = glGetUniformLocation(program, uniModel.c_str());
        if (modelLoc < 0)
        {
            lastError = "Couldn't find 'model' uniform variable";
            std::cout << lastError << std::endl;
            return ErrorConditions::GLUniformVarNotFound;
        }

        colorLoc = glGetUniformLocation(program, uniColor.c_str());
        if (colorLoc < 0)
        {
            lastError = "Couldn't find 'fragColor' uniform variable";
            std::cout << lastError << std::endl;
            return ErrorConditions::GLUniformVarNotFound;
        }

        // One time configuration of projection and view matrix
        GLint projLoc = glGetUniformLocation(program, uniProjection.c_str());
        if (projLoc < 0)
        {
            lastError = "Couldn't find 'projection' uniform variable";
            std::cout << lastError << std::endl;
            return ErrorConditions::GLUniformVarNotFound;
        }

        GLint viewLoc = glGetUniformLocation(program, uniView.c_str());
        if (viewLoc < 0)
        {
            lastError = "Couldn't find 'view' uniform variable";
            std::cout << lastError << std::endl;
            return ErrorConditions::GLUniformVarNotFound;
        }

        int err = 0;
        Matrix4 pm = projection.getMatrix();
        glUniformMatrix4fv(projLoc, GLUniformMatrixCount, GLUniformMatrixTransposed, pm.data());
        err = checkGLError("StaticMonoAtlas::configureUniforms:glUniformMatrix4fv(1)");
        if (err < 0)
            return ErrorConditions::GLFunctionError;

        glUniformMatrix4fv(viewLoc, GLUniformMatrixCount, GLUniformMatrixTransposed, camera.viewspace.data());
        err = checkGLError("StaticMonoAtlas::configureUniforms:glUniformMatrix4fv(2)");
        if (err < 0)
            return ErrorConditions::GLFunctionError;

        return ErrorConditions::None;
    }

    void StaticMonoAtlas::vboBind(int bufferSize, const std::vector<GLfloat> &vertices)
    {
        // the buffer type of a vertex buffer object is GL_ARRAY_BUFFER
        // From this point on any buffer calls we make (on the GL_ARRAY_BUFFER target)
        // will be used to configure the currently bound buffer, which is VBO
        glBindBuffer(GL_ARRAY_BUFFER, vboID);

        // Create and fill buffer
        glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices.data(), GL_DYNAMIC_DRAW);
    }

    void StaticMonoAtlas::eboBind(int bufferSize, const std::vector<GLuint> &indices)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, indices.data(), GL_STATIC_DRAW);
    }

    ErrorConditions StaticMonoAtlas::burn()
    {
        ErrorConditions configureStatus = configure();
        if (configureStatus != ErrorConditions::None)
            return configureStatus;

        shake();

        ErrorConditions bakeStatus = bake();

        return bakeStatus;
    }

    int StaticMonoAtlas::addShape(std::string name, const std::vector<GLfloat> &vertices, std::vector<GLuint> &indices, GLenum mode)
    {
        auto shape = std::make_shared<Shape>();

        shape->id = nextID++;
        shape->name = name;
        shape->dirty = false;
        shape->vertices = vertices;
        shape->indices = indices;
        shape->indicesCount = indices.size();
        shape->primitiveMode = mode;

        shapes.push_back(shape);

        return shape->id;
    }

    shapeShPtr StaticMonoAtlas::getShapeByName(const std::string &name) const
    {
        auto it = std::find_if(shapes.begin(), shapes.end(),
                               [name](shapeShPtr n)
                               { return n->name == name; });
        return it != shapes.end() ? *it : nullptr;
    }

    shapeShPtr StaticMonoAtlas::getShapeById(const int id) const
    {
        auto it = std::find_if(shapes.begin(), shapes.end(),
                               [id](shapeShPtr n)
                               { return n->id == id; });
        return it != shapes.end() ? *it : nullptr;
    }

    void StaticMonoAtlas::use()
    {
        // See opengl wiki as to why "glBindVertexArray(0)" isn't really necessary here:
        // https://www.opengl.org/wiki/Vertex_Specification#Vertex_Buffer_Object
        // Note the line "Changing the GL_ARRAY_BUFFER binding changes nothing about vertex attribute 0..."
        shader.use();
        glBindVertexArray(vaoID);
    }

    void StaticMonoAtlas::unUse()
    {
        glBindVertexArray(UnBindID);
    }

    void StaticMonoAtlas::dispose()
    {
        std::cout << "StaticMonoAtlas::dispose" << std::endl;

        glDeleteVertexArrays(1, &vaoID);
        glDeleteBuffers(1, &eboID);
        glDeleteBuffers(1, &vboID);

        GLuint program = shader.program();
        glDeleteProgram(program);
    }

    void StaticMonoAtlas::setColor(const std::array<GLfloat, 4> &color)
    {
        glUniform4fv(colorLoc, Uniform4vColorCompCount, color.data());
    }

    void StaticMonoAtlas::render(int shapeId, const Matrix4 &model)
    {
        shapeShPtr shape = getShapeById(shapeId);
        if (shape != nullptr)
        {
            // model.data() ==> (const GLfloat *)&model.e[0]
            glUniformMatrix4fv(modelLoc, GLUniformMatrixCount, GLUniformMatrixTransposed, model.data());
            glDrawElements(shape->primitiveMode, shape->indicesCount, GL_UNSIGNED_INT, shape->dataIndicesOffset());
        }
    }

} // namespace Core
