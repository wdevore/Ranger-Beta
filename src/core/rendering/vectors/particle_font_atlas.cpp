#include <iomanip>
#include <bitset>

#include "particle_font_atlas.hpp"
#include "dark_rose.hpp"

namespace Core
{
    ParticleFontAtlas::ParticleFontAtlas()
    {
        name = "ParticleFontAtlas";
        usesEbo = false;
    }

    ParticleFontAtlas::~ParticleFontAtlas()
    {
    }

    ErrorConditions ParticleFontAtlas::initialize()
    {
        shader.specifyShaderFiles("particle_vertex.glsl", "particle_fragment_smooth2.glsl");

        constructGeomerty();

        primitiveMode = GL_POINTS;

        return initialAndBuildShader_();
    }

    void ParticleFontAtlas::constructGeomerty()
    {
        // Create geometry for DarkroseBitmapFont
        bitmapFont.build();

        const int squaresPerSide = 8;
        const float gapSize = 0.06f; // From darkrose_bitmap_font.hpp
        const float unitLength = 1.0f;
        const int numberOfGaps = squaresPerSide - 1;
        const float sideLength = (unitLength - (float)(numberOfGaps)*gapSize) / (float)squaresPerSide;
        const float step = sideLength + gapSize;

        for (char c : DarkroseFont::darkRose_chars)
        {
            // std::cout << "----------------------------------" << std::endl;
            // std::cout << "Char: '" << c << "'" << std::endl;

            uint64_t charBitmap = bitmapFont.getCharBitmap(c);
            // if (charBitmap == 0)
            //     continue;

            int currentVertexCount = 0;
            // The starting index for glDrawArrays is the number of vertices
            // already in the buffer.
            int startVertexIndex = vertices.size() / Core::XYZComponentCount;

            for (int y = 0; y < squaresPerSide; ++y)
            {
                // Extract the row (byte)
                uint8_t row = (charBitmap >> ((squaresPerSide - 1 - y) * 8)) & 0xFF;

                // std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(row) << " = 0b" << std::bitset<8>(row) << " : ";

                for (int x = 0; x < squaresPerSide; ++x)
                {
                    // Check if the bit is set
                    if ((row >> (squaresPerSide - 1 - x)) & 1)
                    {
                        float px = (float)x * step - 0.5f + sideLength / 2.0f;
                        // The bitmap font is defined top-to-bottom, so we flip the Y coordinate.
                        float py = (float)(squaresPerSide - 1 - y) * step - 0.5f + sideLength / 2.0f;
                        // std::cout << "<" << std::showpos << std::fixed << std::setprecision(4) << std::setfill('0') << px << "," << py << "> ";
                        vertices.push_back(px);
                        vertices.push_back(py);
                        vertices.push_back(0.0f);
                        currentVertexCount++;
                    }
                    else
                    {
                        // std::cout << "_________________ ";
                        // std::cout << "<_0.0000,_0.0000> ";
                    }
                }
                // std::cout << std::endl;
            }

            // std::cout << "charVertexData: {" << startVertexIndex << ", " << currentVertexCount << "}" << std::endl;

            charVertexData[c] = {startVertexIndex, currentVertexCount};
        }
    }

    ErrorConditions ParticleFontAtlas::initialAndBuildShader_()
    {
        shader.initialize();
        return shader.build();
    }

    void ParticleFontAtlas::finalize()
    {
        configureUniforms();
        transferToGPU();
    }

    ErrorConditions ParticleFontAtlas::configureUniforms()
    {
        Atlas::configureUniforms();
        GLuint program = shader.program();

        colorLoc = fetchUniformVar(program, uniColor);
        if (colorLoc < 0)
            return Core::ErrorConditions::GLUniformVarNotFound;

        particleSizeLoc = fetchUniformVar(program, uniPointSize);
        if (particleSizeLoc < 0)
            return Core::ErrorConditions::GLUniformVarNotFound;

        return Core::ErrorConditions::None;
    }

    void ParticleFontAtlas::setColor(const std::array<GLfloat, 4> &color)
    {
        glUniform4fv(colorLoc, 1, color.data());
        checkGLError(name + "::setColor");
    }

    void ParticleFontAtlas::setParticleSize(float size)
    {
        glUniform1f(particleSizeLoc, size);
        checkGLError(name + "::setParticleSize");
    }

    void ParticleFontAtlas::renderChar(char character, const Matrix4 &model)
    {

        if (charVertexData.find(character) == charVertexData.end())
            return;

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.data());
        checkGLError(name + "::renderChar glUniformMatrix4fv model");

        auto pair = charVertexData[character];
        glDrawArrays(primitiveMode, pair.first, pair.second);
        checkGLError(name + "::renderChar glDrawArrays");

        // glDisable(GL_PROGRAM_POINT_SIZE);
    }

    void ParticleFontAtlas::renderText(const std::list<char> &characters, const Matrix4 &model)
    {
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.data());
        for (char character : characters)
        {
            if (charVertexData.find(character) == charVertexData.end())
                continue;
            auto pair = charVertexData[character];
            glDrawArrays(primitiveMode, pair.first, pair.second);
        }
        checkGLError(name + "::renderText(1)");

        // glDisable(GL_PROGRAM_POINT_SIZE);
    }

    void ParticleFontAtlas::renderText(const std::list<char> &characters)
    {
        // This must be enabled for gl_PointSize to work in the vertex shader.
        float xIncOffset = 0.0;
        setParticleSize(particleSize);

        for (char character : characters)
        {
            if (charVertexData.find(character) == charVertexData.end())
                continue;

            model.toIdentity();
            model.translate(position.x + xIncOffset, position.y, position.z);
            model.scaleBy(scale);
            xIncOffset += horzOffset;

            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.data());
            checkGLError(name + "::renderText2 glUniformMatrix4fv model");

            auto pair = charVertexData[character];
            glDrawArrays(primitiveMode, pair.first, pair.second);
        }
        checkGLError(name + "::renderText(2)");

        // glDisable(GL_PROGRAM_POINT_SIZE);
    }
} // namespace Core