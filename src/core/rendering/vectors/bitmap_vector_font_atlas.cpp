
#include "bitmap_vector_font_atlas.hpp"

namespace Core
{
    BitmapVectorFontAtlas::BitmapVectorFontAtlas(/* args */)
    {
        name = "BitmapVectorFontAtlas";
    }

    BitmapVectorFontAtlas::~BitmapVectorFontAtlas()
    {
    }

    ErrorConditions BitmapVectorFontAtlas::initialize()
    {
        // mono_vert_ubo contains uniforms for Projecttion and View matricies.
        shader.specifyShaderFiles("mono_vert_ubo.glsl", "mono_fragment.glsl");

        constructGeomerty();

        Core::ShapeGenerator generator = bitmapVecFont.getGenerator();

        primitiveMode = GL_TRIANGLES;

        // Copy all shapes vertex data into Backing store. This is the only set
        // of vertices because all indices refer to it.
        vertices = generator.shape.vertices;

        // Each offset represents a group of indices for a single character
        std::unordered_map<char, std::pair<int, int>> indicesGroupData = bitmapVecFont.getIndicesGroupData();

        for (auto &&pair : indicesGroupData)
        {
            char character = pair.first;
            int indicesGroupCount = pair.second.first;
            // These need to be "remapped" into byte offsets.
            int indiceGroupOffset = pair.second.second * sizeof(GLuint); // This must be in bytes

            // std::cout << "Char: '" << character << "' GroupOffset: " << indiceGroupOffset << " GroupCount: " << indicesGroupCount << std::endl;
            indicesPairData[character] = {indicesGroupCount, indiceGroupOffset};
        }

        indices = generator.shape.indices;

        // -------------- Now construct shader -----------------------------
        ErrorConditions buildStatus = initialAndBuildShader_();

        return buildStatus;
    }

    void BitmapVectorFontAtlas::constructGeomerty()
    {
        // Create geometry for DarkroseBitmapFont
        bitmapVecFont.build();
    }

    ErrorConditions BitmapVectorFontAtlas::initialAndBuildShader_()
    {
        shader.initialize();

        ErrorConditions buildStatus = shader.build();
        if (buildStatus != ErrorConditions::None)
            return buildStatus;

        return ErrorConditions::None;
    }

    void BitmapVectorFontAtlas::finalize()
    {
        configureUniforms();

        transferToGPU();
    }

    ErrorConditions BitmapVectorFontAtlas::configureUniforms()
    {
        Atlas::configureUniforms();

        GLuint program = shader.program();

        colorLoc = fetchUniformVar(program, uniColor);
        if (colorLoc < 0)
            return Core::ErrorConditions::GLUniformVarNotFound;

        return Core::ErrorConditions::None;
    }

    void BitmapVectorFontAtlas::setColor(const std::array<GLfloat, 4> &color)
    {
        glUniform4fv(colorLoc, Uniform4vColorCompCount, color.data());
        Core::checkGLError(name + "::setColor");
    }

    void BitmapVectorFontAtlas::renderChar(char character, const Matrix4 &model)
    {
        glUniformMatrix4fv(modelLoc, GLUniformMatrixCount, GLUniformMatrixTransposed, model.data());

        // TODO !!!!! This is making a copy. We want a reference!
        auto pair = indicesPairData[character];

        // Examples:
        // 30, sizeof(GLuint) * 0 !
        // 24, 4*30 "
        // 96, 4*54 #
        glDrawElements(primitiveMode,
                       pair.first, // Count
                       GL_UNSIGNED_INT,
                       static_cast<void *>(static_cast<char *>(nullptr) + pair.second)); // Offset
        Core::checkGLError("BitmapVectorFontAtlas::renderChar glDrawElements");
    }

    void BitmapVectorFontAtlas::renderText(const std::list<char> &characters, const Matrix4 &model)
    {
        glUniformMatrix4fv(modelLoc, GLUniformMatrixCount, GLUniformMatrixTransposed, model.data());
        Core::checkGLError("BitmapVectorFontAtlas::renderText glUniformMatrix4fv model");

        for (char character : characters)
        {
            auto pair = indicesPairData[character];
            glDrawElements(primitiveMode,
                           pair.first, // Count
                           GL_UNSIGNED_INT,
                           static_cast<void *>(static_cast<char *>(nullptr) + pair.second)); // Offset
            Core::checkGLError("BitmapVectorFontAtlas::renderText glDrawElements");
        }
        checkGLError(name + "::renderText(1)");
    }

    void BitmapVectorFontAtlas::renderText(const std::list<char> &characters)
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
            Core::checkGLError("BitmapVectorFontAtlas::renderText2 glUniformMatrix4fv model");

            glDrawElements(primitiveMode,
                           pair.first, // Count
                           GL_UNSIGNED_INT,
                           static_cast<void *>(static_cast<char *>(nullptr) + pair.second)); // Offset in bytes
            Core::checkGLError("BitmapVectorFontAtlas::renderText glDrawElements");
        }
        checkGLError(name + "::renderText(2)");
    }
} // namespace Core
