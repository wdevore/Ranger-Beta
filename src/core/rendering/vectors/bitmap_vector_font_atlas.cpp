#include <memory>    // std::make_shared
#include <algorithm> // For std::find or std::find_if
#include <utility>

#include <bitmap_vector_font_atlas.hpp>
#include <constants.hpp>
#include <environment.hpp>
#include <shape_generator.hpp>

namespace Core
{
    void BitmapVectorFontAtlas::initialize(environmentShPtr environment)
    {
        BaseAtlas::initialize(environment);
        name = "BitmapVectorFontAtlas";
    }

    ErrorConditions BitmapVectorFontAtlas::configure()
    {
        std::cout << name << "::configure" << std::endl;

        // Load shader pograms
        shader.initialize(environment);
        ErrorConditions buildStatus = shader.build();
        if (buildStatus != ErrorConditions::None)
            return buildStatus;

        return ErrorConditions::None;
    }

    void BitmapVectorFontAtlas::configureFrom(bitmapFontBaseUnqPtr fontBase)
    {
        // Use fontBase to obtain vertices and local-indices

        // TODO we don't really need to take ownership of the fontBase
        // because once we have copied what we need we can discard it.
        // For now we will keep it.
        this->fontBase = std::move(fontBase);

        // Generator contains vertices, Fontbase has indices.
        Core::ShapeGenerator generator = this->fontBase->getGenerator();

        primitiveMode = GL_TRIANGLES;

        // ---------------------------------------------------------
        // Shake
        // ---------------------------------------------------------
        // Copy all shapes vertex data into Backing store. This is the only set
        // of vertices because all indices refer to it.
        for (auto &&vertex : generator.shape.vertices)
            backingShape.vertices.push_back(vertex);

        // We need to simulate what StaticMonoAtlas::shakeShape does but only
        // for indices. For each indice group we simulate adding a shape.

        // Each offset represents a group of indices for a single character
        std::unordered_map<char, std::pair<int, int>> indicesGroupData = this->fontBase->getIndicesGroupCounts();

        // These need to be "remapped" into byte offsets.
        for (auto &&pair : indicesGroupData)
        {
            char character = pair.first;
            int indicesGroupCount = pair.second.first; // This must be in bytes
            int indiceGroupOffset = pair.second.second;

            // std::cout << "Char: '" << character << "' GroupOffset: " << indiceGroupOffset << " GroupCount: " << indicesGroupCount << std::endl;
            indicesPairOffsets[character] = {indicesGroupCount, indiceGroupOffset * sizeof(GLuint)};
        }

        // The index offset is always refering to a position within
        // the vertices array. It is a integer pointer offset where the pointer
        // is defined as an "integer count".
        // We use the current block marker to map each local-index-space into
        // buffer-index-space.
        for (GLuint &index : generator.shape.indices)
            backingShape.indices.push_back(static_cast<GLuint>(index));

        // ---------------------------------------------------------
        // Bake into OpenGL
        // ---------------------------------------------------------
        bake();
    }

    ErrorConditions BitmapVectorFontAtlas::bake()
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
        GLuint vertexSize = static_cast<GLuint>(Core::XYZComponentCount * sizeof(GLfloat));

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

    ErrorConditions BitmapVectorFontAtlas::configureUniforms()
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

    void BitmapVectorFontAtlas::vboBind(int bufferSize, const std::vector<GLfloat> &vertices)
    {
        // the buffer type of a vertex buffer object is GL_ARRAY_BUFFER
        // From this point on any buffer calls we make (on the GL_ARRAY_BUFFER target)
        // will be used to configure the currently bound buffer, which is VBO
        glBindBuffer(GL_ARRAY_BUFFER, vboID);

        // Create and fill buffer
        glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices.data(), GL_DYNAMIC_DRAW);
    }

    void BitmapVectorFontAtlas::eboBind(int bufferSize, const std::vector<GLuint> &indices)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, indices.data(), GL_STATIC_DRAW);
    }

    void BitmapVectorFontAtlas::use()
    {
        // See opengl wiki as to why "glBindVertexArray(0)" isn't really necessary here:
        // https://www.opengl.org/wiki/Vertex_Specification#Vertex_Buffer_Object
        // Note the line "Changing the GL_ARRAY_BUFFER binding changes nothing about vertex attribute 0..."
        shader.use();
        glBindVertexArray(vaoID);
    }

    void BitmapVectorFontAtlas::unUse()
    {
        glBindVertexArray(UnBindID);
    }

    void BitmapVectorFontAtlas::dispose()
    {
        std::cout << "BitmapVectorFontAtlas::dispose" << std::endl;

        glDeleteVertexArrays(1, &vaoID);
        glDeleteBuffers(1, &eboID);
        glDeleteBuffers(1, &vboID);

        GLuint program = shader.program();
        glDeleteProgram(program);
    }

    void BitmapVectorFontAtlas::setColor(const std::array<GLfloat, 4> &color)
    {
        glUniform4fv(colorLoc, Uniform4vColorCompCount, color.data());
    }

    void BitmapVectorFontAtlas::renderChar(char character, std::list<int> offsets, const Matrix4 &model)
    {
        glUniformMatrix4fv(modelLoc, GLUniformMatrixCount, GLUniformMatrixTransposed, model.data());

        // TODO !!!!! This is making a copy. We want a reference!
        auto pair = indicesPairOffsets[character];

        // Examples:
        // 30, sizeof(GLuint) * 0 !
        // 24, 4*30 "
        // 96, 4*54 #
        glDrawElements(primitiveMode,
                       pair.first, // Count
                       GL_UNSIGNED_INT,
                       static_cast<void *>(static_cast<char *>(nullptr) + pair.second)); // Offset
    }

    void BitmapVectorFontAtlas::renderText(std::list<int> offsets, const Matrix4 &model)
    {
        glUniformMatrix4fv(modelLoc, GLUniformMatrixCount, GLUniformMatrixTransposed, model.data());

        auto pair = indicesPairOffsets['#'];
        glDrawElements(primitiveMode,
                       pair.first, // Count
                       GL_UNSIGNED_INT,
                       static_cast<void *>(static_cast<char *>(nullptr) + pair.second)); // Offset
    }

    /// @brief This is used by special nodes that don't render anything but instead
    /// manipulate the node's transform, for example, ZoomNode.
    /// @param model
    void BitmapVectorFontAtlas::render(const Matrix4 &model)
    {
        glUniformMatrix4fv(modelLoc, GLUniformMatrixCount, GLUniformMatrixTransposed, model.data());
    }

} // namespace Core
