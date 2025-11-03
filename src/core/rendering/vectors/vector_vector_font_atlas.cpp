
#include "bitmap_vector_font_atlas.hpp"
#include "vector_vector_font_atlas.hpp"
namespace Core
{
    VectorVectorFontAtlas::VectorVectorFontAtlas(/* args */)
    {
        name = "VectorVectorFontAtlas";
    }

    VectorVectorFontAtlas::~VectorVectorFontAtlas()
    {
    }

    ErrorConditions VectorVectorFontAtlas::initialize(const std::unordered_map<char, std::list<float>> &fontData)
    {
        // mono_vert_ubo contains uniforms for Projecttion and View matricies.
        shader.specifyShaderFiles("mono_vert_ubo.glsl", "mono_fragment.glsl");

        constructGeomerty(fontData);

        primitiveMode = GL_LINES;

        // -------------- Now construct shader -----------------------------
        ErrorConditions buildStatus = initialAndBuildShader_();

        return buildStatus;
    }

    void VectorVectorFontAtlas::constructGeomerty(const std::unordered_map<char, std::list<float>> &fontData)
    {
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
            auto inVertices = charPairs.second;
            int inVerticesSize = inVertices.size();

            // std::cout << "'" << character << "' indices size: " << indices.size() << ", inVertices (" << inVerticesSize << "): ";

            // for (auto &vertex : inVertices)
            //     std::cout << vertex << ", ";
            // std::cout << std::endl;

            // Pack the vertices into the backing buffer.
            // int y = 0;
            // for (auto &vertex : inVertices)
            // {
            //     if (y % 3 == 0)
            //     vertices.push_back(vertex);
            // }
            vertices.insert(vertices.end(), inVertices.begin(), inVertices.end());

            // Build and pack indices into backing buffer. The indices are formed
            // using the rule that all vertices are paired.

            int indicesCount = inVerticesSize / Core::XYZComponentCount;
            // std::cout << "inVerticesSize: " << indicesCount << " :: ";

            for (size_t i = 0; i < indicesCount; i++)
            {
                // std::cout << i + verticesBlockOffset << ", ";
                indices.push_back(static_cast<GLuint>(i + verticesBlockOffset));
            }
            // std::cout << std::endl;

            // This is the important rendering information.
            //                         Count --|               |--- Offset in bytes
            indicesPairData[character] = {indicesCount, indicesOffset * sizeof(GLuint)};

            indicesOffset += indicesCount;

            // std::cout << "Pair Data:: {" << indicesPairData[character].first << ", " << indicesPairData[character].second << "}" << std::endl;

            verticesBlockOffset = static_cast<GLuint>(vertices.size() / Core::XYZComponentCount);
            // std::cout << "Next verticesBlockOffset: " << verticesBlockOffset << std::endl;

            // std::cout << "------------------------------------------------" << std::endl;
        }
    }

    ErrorConditions VectorVectorFontAtlas::initialAndBuildShader_()
    {
        shader.initialize();

        ErrorConditions buildStatus = shader.build();
        if (buildStatus != ErrorConditions::None)
            return buildStatus;

        return ErrorConditions::None;
    }

    void VectorVectorFontAtlas::finalize()
    {
        configureUniforms();

        transferToGPU();
    }

    ErrorConditions VectorVectorFontAtlas::configureUniforms()
    {
        Atlas::configureUniforms();

        GLuint program = shader.program();

        colorLoc = fetchUniformVar(program, uniColor);
        if (colorLoc < 0)
            return Core::ErrorConditions::GLUniformVarNotFound;

        return Core::ErrorConditions::None;
    }

    void VectorVectorFontAtlas::setColor(const std::array<GLfloat, 4> &color)
    {
        glUniform4fv(colorLoc, Uniform4vColorCompCount, color.data());
        Core::checkGLError(name + "::setColor");
    }

    void VectorVectorFontAtlas::renderChar(char character, const Matrix4 &model)
    {
        glUniformMatrix4fv(modelLoc, GLUniformMatrixCount, GLUniformMatrixTransposed, model.data());

        // TODO !!!!! This is making a copy. We want a reference!
        auto pair = indicesPairData[character];

        glDrawElements(primitiveMode,
                       pair.first, // Count
                       GL_UNSIGNED_INT,
                       static_cast<void *>(static_cast<char *>(nullptr) + pair.second)); // Offset
        Core::checkGLError("VectorVectorFontAtlas::renderChar glDrawElements");
    }

    void VectorVectorFontAtlas::renderText(const std::list<char> &characters, const Matrix4 &model)
    {
        glUniformMatrix4fv(modelLoc, GLUniformMatrixCount, GLUniformMatrixTransposed, model.data());
        Core::checkGLError("VectorVectorFontAtlas::renderText glUniformMatrix4fv model");

        for (char character : characters)
        {
            auto pair = indicesPairData[character];
            glDrawElements(primitiveMode,
                           pair.first, // Count
                           GL_UNSIGNED_INT,
                           static_cast<void *>(static_cast<char *>(nullptr) + pair.second)); // Offset
            Core::checkGLError("VectorVectorFontAtlas::renderText glDrawElements");
        }
        checkGLError(name + "::renderText(1)");
    }

    void VectorVectorFontAtlas::renderText(const std::list<char> &characters)
    {
        float xIncOffset = 0.0;

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
            Core::checkGLError("VectorVectorFontAtlas::renderText2 glUniformMatrix4fv model");

            glDrawElements(primitiveMode,
                           pair.first, // Count
                           GL_UNSIGNED_INT,
                           static_cast<void *>(static_cast<char *>(nullptr) + pair.second)); // Offset in bytes
            Core::checkGLError("VectorVectorFontAtlas::renderText glDrawElements");
        }
        checkGLError(name + "::renderText(2)");
    }
} // namespace Core
