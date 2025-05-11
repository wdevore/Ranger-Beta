#include <memory>    // std::make_shared
#include <algorithm> // For std::find or std::find_if

#include <dynamic_mono_atlas.hpp>
#include <constants.hpp>

namespace Core
{
    ErrorConditions DynamicMonoAtlas::configure()
    {
        // Load shader pograms
        ErrorConditions buildStatus = shader.build();
        if (buildStatus != ErrorConditions::None)
            return buildStatus;

        return ErrorConditions::None;
    }

    ErrorConditions DynamicMonoAtlas::shake()
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
        int vertexOffset = 0;

        for (auto &&shape : shapes)
        {
            shape->indicesOffset = indicesOffset;
            shape->vertexOffset = vertexOffset;

            vertexOffset += shape->vertices.size();                  // count
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

    ErrorConditions DynamicMonoAtlas::bake()
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
        vboBufferSize = backingShape.vertices.size() * Core::XYZComponentCount * sizeof(GLfloat);
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
        const GLuint positionIndex = 0;

        // We can link the attribute position with the data in the vertexData array
        glVertexAttribPointer(positionIndex, GLuint(Core::XYZComponentCount), GL_FLOAT, false, vertexSize, (GLvoid *)(0));

        // ----------------------------------------------
        // END VAO Scope
        // ----------------------------------------------
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        ErrorConditions configureUniStatus = configureUniforms();

        return configureUniStatus;
    }

    ErrorConditions DynamicMonoAtlas::configureUniforms()
    {
        shader.use();

        GLuint program = shader.program();

        modelLoc = glGetUniformLocation(program, uniModel.c_str());
        if (modelLoc < 0)
        {
            lastError = "Couldn't find 'model' uniform variable";
            return ErrorConditions::GLUniformVarNotFound;
        }

        colorLoc = glGetUniformLocation(program, uniColor.c_str());
        if (colorLoc < 0)
        {
            lastError = "Couldn't find 'fragColor' uniform variable";
            return ErrorConditions::GLUniformVarNotFound;
        }

        // One time configuration of projection and view matrix
        GLint projLoc = glGetUniformLocation(program, uniProjection.c_str());
        if (projLoc < 0)
        {
            lastError = "Couldn't find 'projection' uniform variable";
            return ErrorConditions::GLUniformVarNotFound;
        }

        GLint viewLoc = glGetUniformLocation(program, uniView.c_str());
        if (viewLoc < 0)
        {
            lastError = "Couldn't find 'view' uniform variable";
            return ErrorConditions::GLUniformVarNotFound;
        }

        Matrix4 pm = projection.getMatrix();
        glUniformMatrix4fv(projLoc, GLUniformMatrixCount, GLUniformMatrixTransposed, pm.data());

        glUniformMatrix4fv(viewLoc, GLUniformMatrixCount, GLUniformMatrixTransposed, viewspace.data());

        return ErrorConditions::None;
    }

    void DynamicMonoAtlas::vboBind(int bufferSize, const std::vector<GLfloat> &vertices)
    {
        // the buffer type of a vertex buffer object is GL_ARRAY_BUFFER
        // From this point on any buffer calls we make (on the GL_ARRAY_BUFFER target)
        // will be used to configure the currently bound buffer, which is VBO
        glBindBuffer(GL_ARRAY_BUFFER, vboID);

        // Create and fill buffer
        glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices.data(), GL_DYNAMIC_DRAW);
    }

    void DynamicMonoAtlas::eboBind(int bufferSize, const std::vector<GLuint> &indices)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, indices.data(), GL_STATIC_DRAW);
    }

    ErrorConditions DynamicMonoAtlas::burn()
    {
        ErrorConditions configureStatus = configure();
        if (configureStatus != ErrorConditions::None)
            return configureStatus;

        shake();

        ErrorConditions bakeStatus = bake();

        return bakeStatus;
    }

    int DynamicMonoAtlas::addShape(std::string name, const std::vector<GLfloat> &vertices, std::vector<GLuint> &indices, GLenum mode)
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

    shapeShPtr DynamicMonoAtlas::getShapeByName(const std::string &name) const
    {
        auto it = std::find_if(shapes.begin(), shapes.end(),
                               [name](shapeShPtr n)
                               { return n->name == name; });
        return it != shapes.end() ? *it : nullptr;
    }

    shapeShPtr DynamicMonoAtlas::getShapeById(const int id) const
    {
        auto it = std::find_if(shapes.begin(), shapes.end(),
                               [id](shapeShPtr n)
                               { return n->id == id; });
        return it != shapes.end() ? *it : nullptr;
    }

    void DynamicMonoAtlas::use()
    {
        shader.use();
        glBindVertexArray(vaoID);
    }

    void DynamicMonoAtlas::unUse()
    {
        glBindVertexArray(UnBindID);
    }

    void DynamicMonoAtlas::setColor(const std::array<GLfloat, 4> &color)
    {
        glUniform4fv(colorLoc, Uniform4vColorCompCount, color.data());
    }

    void DynamicMonoAtlas::update()
    {
        // Copy entire buffer even if just one element changed.
        if (dirty)
        {
            // copy changed shape(s) to backing buffer
            copy();

            // Now copy from backing buffer to GL buffer
            glBindBuffer(GL_ARRAY_BUFFER, vboID);
            // backingShape.vertices.data() ==> (GLvoid *)&backingShape.vertices[0]
            glBufferSubData(GL_ARRAY_BUFFER, GLSubDataOffset, vboBufferSize, backingShape.vertices.data());
            glBindBuffer(GL_ARRAY_BUFFER, UnBindID);
            dirty = false;
        }
    }

    void DynamicMonoAtlas::copy()
    {
        for (auto &&shape : shapes)
        {
            if (shape->dirty)
            {
                int i{0};
                for (auto &&v : shape->vertices)
                {
                    backingShape.vertices[shape->vertexOffset + i] = v;
                    i++;
                }
            }
        }
    }

    void DynamicMonoAtlas::setShapeVertex(double x, double y, int index, int shapeId)
    {
        shapeShPtr shape = getShapeById(shapeId);

        if (shape != nullptr)
        {
            int i = index * VertexStride;
            shape->vertices[i] = x;
            shape->vertices[i + 1] = y;
            shape->dirty = true;
            dirty = true;
        }
    }

    void DynamicMonoAtlas::setVertex(double x, double y, int index)
    {
        int i = index * VertexStride;
        backingShape.vertices[i] = x;
        backingShape.vertices[i + 1] = y;
        backingShape.dirty = true;
        dirty = true;
    }

    void DynamicMonoAtlas::render(int shapeId, const Matrix4 &model)
    {
        shapeShPtr shape = getShapeById(shapeId);

        // model.data() ==> (const GLfloat *)&model.e[0]
        glUniformMatrix4fv(modelLoc, GLUniformMatrixCount, GLUniformMatrixTransposed, model.data());
        glDrawElements(shape->primitiveMode, shape->indicesCount, GL_UNSIGNED_INT, shape->dataIndicesOffset());
    }

} // namespace Core
