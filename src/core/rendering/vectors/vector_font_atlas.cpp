#include <memory>    // std::make_shared
#include <algorithm> // For std::find or std::find_if
#include <utility>
#include <unordered_map>
#include <list>
#include <vector_font_atlas.hpp>
#include <shape_generator.hpp>

namespace Core
{
    void VectorFontAtlas::initialize(const int deviceWidth, const int deviceHeight)
    {
        name = "VectorFontAtlas";
    }

    ErrorConditions VectorFontAtlas::configure(const std::unordered_map<char, std::list<float>> &fontData)
    {
        std::cout << name << "::configure" << std::endl;

        // By reserving memory we improve efficiency and minimize any kind
        // scrambling by the allocator. I shouldn't rely on this strategy.
        indicesPairData.reserve(fontData.size());

        // Load shader pograms
        _loadShaders();

        primitiveMode = GL_LINES;

        int verticesBlockOffset{0};
        /// @brief This offset is byte offsets not integer numbers.
        /// It represents a "index pointer" where the next group of indices
        /// starts.
        int indicesOffset{0};

        // ---------------------------------------------------------
        // Vertices
        // ---------------------------------------------------------
        // As the vertices are "packed" against each other we need to track
        // the start of each group so that when building the indices we start
        // with.
        // We treat each character as a separate shape
        for (auto &charPairs : fontData)
        {
            char character = charPairs.first;
            std::list<float> vertices = charPairs.second;
            std::cout << "'" << character << "' indices size: " << backingShape.indices.size() << ", vertices (" << vertices.size() << "): ";

            for (auto &vertex : vertices)
                std::cout << vertex << ", ";
            std::cout << std::endl;

            // Pack the vertices into the backing buffer.
            backingShape.vertices.insert(backingShape.vertices.end(), vertices.begin(), vertices.end());

            // Build and pack indices into backing buffer. The indices are formed
            // using the rule that all vertices are paired.

            int indicesCount = vertices.size() / Core::XYZComponentCount;
            std::cout << "Indices: " << indicesCount << " :: ";

            for (size_t i = 0; i < indicesCount; i++)
            {
                std::cout << i + verticesBlockOffset << ", ";
                backingShape.indices.push_back(static_cast<GLuint>(i + verticesBlockOffset));
            }
            std::cout << std::endl;

            // This is the important rendering information.
            //                         Count --|               |--- Offset in bytes
            indicesPairData[character] = {indicesCount, indicesOffset * sizeof(GLuint)};

            indicesOffset += indicesCount;

            std::cout << "Pair Data:: {" << indicesPairData[character].first << ", " << indicesPairData[character].second << "}" << std::endl;

            verticesBlockOffset = static_cast<GLuint>(backingShape.vertices.size() / Core::XYZComponentCount);
            std::cout << "Next verticesBlockOffset: " << verticesBlockOffset << std::endl;

            std::cout << "------------------------------------------------" << std::endl;
        }

        // ---------------------------------------------------------
        // Bake into OpenGL
        // ---------------------------------------------------------
        bake();

        return ErrorConditions::None;
    }

    ErrorConditions VectorFontAtlas::bake()
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
        // int vboBufferSize = backingShape.vertices.size() * Core::XYZComponentCount * sizeof(GLfloat);
        int vboBufferSize = backingShape.vertices.size() * sizeof(GLfloat);
        int eboBufferSize = backingShape.indices.size() * sizeof(GLuint);

        if (vboBufferSize == 0 || eboBufferSize == 0)
        {
            return ErrorConditions::GLBuffersSize;
        }

        std::cout << "Vbo buffer size: " << vboBufferSize << std::endl;
        std::cout << "Ebo buffer size: " << eboBufferSize << std::endl;

        vboBind(vboBufferSize, backingShape.vertices);

        eboBind(eboBufferSize, backingShape.indices);

        // A single vertex size is how many components per vertex * size of component.
        GLuint vertexSize = static_cast<GLuint>(Core::XYZComponentCount * sizeof(GLfloat));
        std::cout << "Vertex size: " << vertexSize << std::endl;

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
        glEnableVertexAttribArray(Core::VertexAttribIndex);
        glBindBuffer(GL_ARRAY_BUFFER, UnBindID);
        glBindVertexArray(UnBindID);
        // NOTE: Don't unbind the EBO while the VAO is active,
        // or the EBO's state will be lost.
        // The EBO is managed by the VAO's state.

        ErrorConditions configureUniStatus = configureUniforms();

        if (configureUniStatus != ErrorConditions::None)
            return configureUniStatus;

        // We don't need the backing shape resources anymore now that data
        // has been uploaded to the GPU.
        backingShape.vertices.clear();
        backingShape.indices.clear();

        return ErrorConditions::None;
    }

    ErrorConditions VectorFontAtlas::configureUniforms()
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

        // int err = 0;
        // Matrix4 pm = projection.getMatrix();
        // glUniformMatrix4fv(projLoc, GLUniformMatrixCount, GLUniformMatrixTransposed, pm.data());
        // err = checkGLError(name + "::configureUniforms:glUniformMatrix4fv(1)");
        // if (err < 0)
        //     return ErrorConditions::GLFunctionError;

        // glUniformMatrix4fv(viewLoc, GLUniformMatrixCount, GLUniformMatrixTransposed, environment->camera.viewspace.data());
        // err = checkGLError(name + "::configureUniforms:glUniformMatrix4fv(2)");
        // if (err < 0)
        //     return ErrorConditions::GLFunctionError;

        return ErrorConditions::None;
    }

    void VectorFontAtlas::vboBind(int bufferSize, const std::vector<GLfloat> &vertices)
    {
        // the buffer type of a vertex buffer object is GL_ARRAY_BUFFER
        // From this point on any buffer calls we make (on the GL_ARRAY_BUFFER target)
        // will be used to configure the currently bound buffer, which is VBO
        glBindBuffer(GL_ARRAY_BUFFER, vboID);

        // Create and fill buffer
        glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices.data(), GL_STATIC_DRAW);
    }

    void VectorFontAtlas::eboBind(int bufferSize, const std::vector<GLuint> &indices)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, indices.data(), GL_STATIC_DRAW);
    }

    ErrorConditions VectorFontAtlas::_loadShaders()
    {
        shader.initialize();
        ErrorConditions buildStatus = shader.build();

        return buildStatus;
    }

    void VectorFontAtlas::use()
    {
        glBindVertexArray(vaoID);

        // See opengl wiki as to why "glBindVertexArray(0)" isn't really necessary here:
        // https://www.opengl.org/wiki/Vertex_Specification#Vertex_Buffer_Object
        // Note the line "Changing the GL_ARRAY_BUFFER binding changes nothing about vertex attribute 0..."
        shader.use();
    }

    void VectorFontAtlas::unUse()
    {
        glBindVertexArray(UnBindID);
    }

    void VectorFontAtlas::dispose()
    {
        std::cout << name << "::dispose" << std::endl;

        glDeleteVertexArrays(1, &vaoID);
        glDeleteBuffers(1, &eboID);
        glDeleteBuffers(1, &vboID);

        GLuint program = shader.program();
        glDeleteProgram(program);
    }

    void VectorFontAtlas::setColor(const std::array<GLfloat, 4> &color)
    {
        glUniform4fv(colorLoc, Uniform4vColorCompCount, color.data());
    }

    void VectorFontAtlas::renderChar(char character, const Matrix4 &model)
    {
        glUniformMatrix4fv(modelLoc, GLUniformMatrixCount, GLUniformMatrixTransposed, model.data());

        // TODO !!!!! This is making a copy. We want a reference!
        auto pair = indicesPairData[character];

        // glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, static_cast<void *>(static_cast<char *>(nullptr) + 0));

        glDrawElements(primitiveMode,
                       pair.first, // Count
                       GL_UNSIGNED_INT,
                       static_cast<void *>(static_cast<char *>(nullptr) + pair.second)); // Offset

        // glDrawElements(primitiveMode,
        //                8, // Count
        //                GL_UNSIGNED_INT,
        //                static_cast<void *>(static_cast<char *>(nullptr) + 0)); // Offset
    }

    void VectorFontAtlas::renderText(const std::list<char> &characters, const Matrix4 &model)
    {
        glUniformMatrix4fv(modelLoc, GLUniformMatrixCount, GLUniformMatrixTransposed, model.data());

        for (char character : characters)
        {
            auto pair = indicesPairData[character];
            glDrawElements(primitiveMode,
                           pair.first, // Count
                           GL_UNSIGNED_INT,
                           static_cast<void *>(static_cast<char *>(nullptr) + pair.second)); // Offset
        }
    }

    void VectorFontAtlas::renderText(const std::list<char> &characters)
    {
        xIncOffset = 0.0;

        for (char character : characters)
        {
            auto pair = indicesPairData[character];
            model.toIdentity();
            model.translate(position.x + xIncOffset, position.y, position.z);
            // If the font is upside down we can scale the Y axis by a negative.
            // Instead I fixed the generator to iterate each from bottom to top.
            model.scaleBy(scale);
            xIncOffset += horzOffset;

            glUniformMatrix4fv(modelLoc, GLUniformMatrixCount, GLUniformMatrixTransposed, model.data());

            glDrawElements(primitiveMode,
                           pair.first, // Count
                           GL_UNSIGNED_INT,
                           static_cast<void *>(static_cast<char *>(nullptr) + pair.second)); // Offset in bytes
        }
    }

    /// @brief This is used by special nodes that don't render anything but instead
    /// manipulate the node's transform, for example, ZoomNode.
    /// @param model
    void VectorFontAtlas::render(const Matrix4 &model)
    {
        glUniformMatrix4fv(modelLoc, GLUniformMatrixCount, GLUniformMatrixTransposed, model.data());
    }

} // namespace Core
