#include <memory>    // std::make_shared
#include <algorithm> // For std::find or std::find_if

#include <static_mono_atlas.hpp>
#include <constants.hpp>
#include <environment.hpp>

namespace Core
{
    void StaticMonoAtlas::initialize(environmentShPtr environment)
    {
        BaseAtlas::initialize(environment);
        name = "StaticMonoAtlas";
    }

    ErrorConditions StaticMonoAtlas::configure()
    {
        std::cout << name << "::configure" << std::endl;

        // Load shader pograms
        shader.initialize(environment);
        ErrorConditions buildStatus = shader.build();
        if (buildStatus != ErrorConditions::None)
            return buildStatus;

        return ErrorConditions::None;
    }

    /// @brief Called by environment post initialize
    /// @param andShake
    /// @return
    ErrorConditions StaticMonoAtlas::burn(bool andShake)
    {
        std::cout << name << "::burn" << std::endl;

        ErrorConditions configureStatus = configure();
        if (configureStatus != ErrorConditions::None)
            return configureStatus;

        if (andShake)
            shake();

        ErrorConditions bakeStatus = bake();

        return bakeStatus;
    }

    ErrorConditions StaticMonoAtlas::shake()
    {
        // ---------------------------------------------------------
        // Collect all vertices and indices for buffers
        // ---------------------------------------------------------
        for (auto &&shape : shapes)
        {
            shakeShape(*shape);
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

        eboBind(eboBufferSize, backingShape.indices);

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

        // We don't need the backing shape resources anymore now that data
        // has been uploaded to the GPU.
        backingShape.vertices.clear();
        backingShape.indices.clear();

        return ErrorConditions::None;
    }

    ErrorConditions StaticMonoAtlas::configureUniforms()
    {
        std::cout << name << "::configureUniforms" << std::endl;

        shader.use();

        GLuint program = shader.program();

        modelLoc = glGetUniformLocation(program, uniModel.c_str());
        if (modelLoc < 0)
        {
            lastError = "Couldn't find 'model' uniform variable";
            std::cout << name << ": " << lastError << std::endl;
            return ErrorConditions::GLUniformVarNotFound;
        }

        colorLoc = glGetUniformLocation(program, uniColor.c_str());
        if (colorLoc < 0)
        {
            lastError = "Couldn't find 'fragColor' uniform variable";
            std::cout << name << ": " << lastError << std::endl;
            return ErrorConditions::GLUniformVarNotFound;
        }

        // One time configuration of projection and view matrix
        GLint projLoc = glGetUniformLocation(program, uniProjection.c_str());
        if (projLoc < 0)
        {
            lastError = "Couldn't find 'projection' uniform variable";
            std::cout << name << ": " << lastError << std::endl;
            return ErrorConditions::GLUniformVarNotFound;
        }

        GLint viewLoc = glGetUniformLocation(program, uniView.c_str());
        if (viewLoc < 0)
        {
            lastError = "Couldn't find 'view' uniform variable";
            std::cout << name << ": " << lastError << std::endl;
            return ErrorConditions::GLUniformVarNotFound;
        }

        int err = 0;
        Matrix4 pm = projection.getMatrix();
        glUniformMatrix4fv(projLoc, GLUniformMatrixCount, GLUniformMatrixTransposed, pm.data());
        err = checkGLError(name + "::configureUniforms:glUniformMatrix4fv(1)");
        if (err < 0)
            return ErrorConditions::GLFunctionError;

        glUniformMatrix4fv(viewLoc, GLUniformMatrixCount, GLUniformMatrixTransposed, environment->camera.viewspace.data());
        err = checkGLError(name + "::configureUniforms:glUniformMatrix4fv(2)");
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

    int StaticMonoAtlas::addShape(const Shape &shape)
    {
        auto shapeShPtr = std::make_shared<Shape>();

        shapeShPtr->id = nextID++;
        shapeShPtr->name = shape.name;
        shapeShPtr->dirty = false;
        shapeShPtr->vertices = shape.vertices;
        shapeShPtr->indices = shape.indices;
        shapeShPtr->indicesCount = shape.indices.size();
        shapeShPtr->primitiveMode = shape.primitiveMode;

        shapes.push_back(shapeShPtr);

        return shape.id;
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

    int StaticMonoAtlas::addShapeAndShake(Shape &shape)
    {
        addShape(shape);
        shakeShape(shape);
        return verticesBlockOffset;
    }

    int StaticMonoAtlas::shakeShape(Shape &shape)
    {
        // std::cout << "Name: " << shape.name << std::endl;

        // std::cout << "indicesByteOffset: " << indicesByteOffset << std::endl;
        // Assign current offset to this shape. Each shape has a group of indices
        // --at a starting offset position-- within the EBO buffer and assigns
        // it to the shape.
        shape.indicesOffset = indicesByteOffset;

        // Now calc the offset for the next potential shape shake call.
        // std::cout << "Total indices: " << shape.indices.size() << std::endl;
        // std::cout << "Total vertices: " << shape.vertices.size() / Core::XYZComponentCount << std::endl;
        // std::cout << "Total vertex floats: " << shape.vertices.size() << std::endl;
        // std::cout << "Size of GLuint: " << sizeof(GLuint) << std::endl;

        indicesByteOffset += shape.indices.size() * sizeof(GLuint); // bytes
        // std::cout << "Next indicesByteOffset: " << indicesByteOffset << std::endl;

        // Copy all shapes vertex data into Backing store
        for (auto &&vertex : shape.vertices)
            backingShape.vertices.push_back(vertex);

        // The index offset is always refering to a position within
        // the vertices array. It is a integer pointer offset where the pointer
        // is defined as an "integer count".
        // We use the current block marker to map each local-index-space into
        // buffer-index-space.
        for (GLuint &i : shape.indices)
        {
            // std::cout << "i: " << i << ", offset: " << (i + verticesBlockOffset) << std::endl;
            backingShape.indices.push_back(static_cast<GLuint>(i + verticesBlockOffset));
        }

        // std::cout << "verticesBlockOffset: " << verticesBlockOffset << std::endl;

        // Calc the next block offset
        // Offset the indices based on the vertex block position as a "component count".
        // We divide the total # of vertices by how large a vertex specification is.
        // A vertex is specified using 3 components x,y,z. For example, a retangle has
        // 4 vertices where each has 3 floats = 4 * 3 = 12.
        // std::cout << "Total vertices in backing: " << backingShape.vertices.size() << std::endl;

        verticesBlockOffset = static_cast<GLuint>(backingShape.vertices.size() / Core::XYZComponentCount);
        // std::cout << "Next verticesBlockOffset: " << verticesBlockOffset << std::endl;

        // std::cout << "-------------------------------------------------" << std::endl;

        return verticesBlockOffset;
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

    int StaticMonoAtlas::getIndicesOffset() const
    {
        return indicesByteOffset;
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

    /// @brief This is used by special nodes that don't render anything but instead
    /// manipulate the node, for example, ZoomNode.
    /// @param model
    void StaticMonoAtlas::render(const Matrix4 &model)
    {
        glUniformMatrix4fv(modelLoc, GLUniformMatrixCount, GLUniformMatrixTransposed, model.data());
    }

} // namespace Core
