#include <algorithm> // For std::find or std::find_if

#include "shapes_atlas.hpp"

namespace Core
{
    ShapesAtlas::ShapesAtlas(/* args */)
    {
        name = "ShapesAtlas";
    }

    ShapesAtlas::~ShapesAtlas()
    {
    }

    ErrorConditions ShapesAtlas::initialAndBuildShader_()
    {
        shader.initialize();

        ErrorConditions buildStatus = shader.build();
        if (buildStatus != ErrorConditions::None)
            return buildStatus;

        return ErrorConditions::None;
    }

    // This is the same as "shaking" a shape.
    int ShapesAtlas::transferShapeToBacking_(shapeShPtr shape)
    {
        // std::cout << "Name: " << shape.name << std::endl;

        // std::cout << "indicesByteOffset: " << indicesByteOffset << std::endl;
        // Assign current offset to this shape. Each shape has a group of indices
        // --at a starting offset position-- within the EBO buffer and assigns
        // it to the shape.
        shape->indicesOffset = indicesByteOffset;

        // Now calc the offset for the next potential shape shake call.
        // std::cout << "Total indices: " << shape.indices.size() << std::endl;
        // std::cout << "Total vertices: " << shape.vertices.size() / Core::XYZComponentCount << std::endl;
        // std::cout << "Total vertex floats: " << shape.vertices.size() << std::endl;
        // std::cout << "Size of GLuint: " << sizeof(GLuint) << std::endl;

        indicesByteOffset += shape->indices.size() * sizeof(GLuint); // bytes
        // std::cout << "Next indicesByteOffset: " << indicesByteOffset << std::endl;

        // Copy all shapes vertex data into Backing store
        for (auto &&vertex : shape->vertices)
            vertices.push_back(vertex);

        // The index offset is always refering to a position within
        // the vertices array. It is a integer pointer offset where the pointer
        // is defined as an "integer count".
        // We use the current block marker to map each local-index-space into
        // buffer-index-space.
        for (GLuint &i : shape->indices)
        {
            // std::cout << "i: " << i << ", offset: " << (i + verticesBlockOffset) << std::endl;
            indices.push_back(static_cast<GLuint>(i + verticesBlockOffset));
        }

        // std::cout << "verticesBlockOffset: " << verticesBlockOffset << std::endl;

        // Calc the next block offset
        // Offset the indices based on the vertex block position as a "component count".
        // We divide the total # of vertices by how large a vertex specification is.
        // A vertex is specified using 3 components x,y,z. For example, a retangle has
        // 4 vertices where each has 3 floats = 4 * 3 = 12.
        // std::cout << "Total vertices in backing: " << backingShape.vertices.size() << std::endl;

        verticesBlockOffset = static_cast<GLuint>(vertices.size() / Core::XYZComponentCount);
        // std::cout << "Next verticesBlockOffset: " << verticesBlockOffset << std::endl;

        // std::cout << "-------------------------------------------------" << std::endl;

        return verticesBlockOffset;
    }

    ErrorConditions ShapesAtlas::initialize()
    {
        // mono_vert_ubo contains uniforms for Project and View matricies.
        shader.specifyShaderFiles("mono_vert_ubo.glsl", "mono_fragment.glsl");

        ErrorConditions buildStatus = initialAndBuildShader_();

        return buildStatus;
    }

    // L-value overload: copies the shape
    const shapeShPtr ShapesAtlas::addShape(const Shape &shape)
    {
        // Create a new shape by copying the provided one,
        // then modify the properties specific to the atlas's version.
        auto shapeShPtr = std::make_shared<Shape>(shape); // Use copy constructor

        shapeShPtr->dirty = false; // The atlas considers it clean upon adding
        shapeShPtr->indicesCount = shape.indices.size();

        shapes.push_back(shapeShPtr);

        return shapeShPtr;
    }

    // ----------------------------------------------------------------
    // ------------- Shape loaders via generators ---------------------
    // ----------------------------------------------------------------
    // Once all shapes have been added you need to call finalize()
    int ShapesAtlas::addSquare(ShapeControls alignment, ShapeControls fillType)
    {
        generator.generateUnitRectangle(alignment, fillType);
        addShape(generator.shape);
        return generator.shape.id;
    }

    int ShapesAtlas::addTriangle(ShapeControls alignment, ShapeControls fillType)
    {
        generator.generateUnitTriangle(alignment, fillType);
        addShape(generator.shape);
        return generator.shape.id;
    }

    int ShapesAtlas::addHLine()
    {
        generator.generateUnitHLine();
        addShape(generator.shape);
        return generator.shape.id;
    }

    int ShapesAtlas::addVLine()
    {
        generator.generateUnitVLine();
        addShape(generator.shape);
        return generator.shape.id;
    }

    int ShapesAtlas::addPlus()
    {
        generator.generateUnitPlus();
        addShape(generator.shape);
        return generator.shape.id;
    }

    int ShapesAtlas::addCircle(int segmentCount, ShapeControls fillType)
    {
        generator.generateUnitCircle(segmentCount, fillType);
        addShape(generator.shape);
        return generator.shape.id;
    }

    int ShapesAtlas::addArc(float startAngle, float endAngle,
                            int segmentCount, ShapeControls fillType)
    {
        generator.generateUnitArc(startAngle, endAngle, segmentCount, fillType);
        addShape(generator.shape);
        return generator.shape.id;
    }

    void ShapesAtlas::finalize()
    {
        configureUniforms();

        // First transfer shape geometry to backing collections for use in bind()
        for (auto &&shape : shapes)
            transferShapeToBacking_(shape);

        transferToGPU();
    }

    ErrorConditions ShapesAtlas::configureUniforms()
    {
        Atlas::configureUniforms();

        GLuint program = shader.program();

        colorLoc = fetchUniformVar(program, uniColor);
        if (colorLoc < 0)
            return Core::ErrorConditions::GLUniformVarNotFound;

        return Core::ErrorConditions::None;
    }

    const shapeShPtr ShapesAtlas::getShapeById(const int id) const
    {
        auto it = std::find_if(shapes.begin(), shapes.end(),
                               [id](shapeShPtr n)
                               { return n->id == id; });
        return it != shapes.end() ? *it : nullptr;
    }

    // Not often used because of the need for a lookup.
    void ShapesAtlas::render(int shapeId, const Matrix4 &model)
    {
        auto shape = getShapeById(shapeId);
        if (shape != nullptr)
        {
            // model.data() ==> (const GLfloat *)&model.e[0]
            glUniformMatrix4fv(modelLoc, GLUniformMatrixCount, GLUniformMatrixTransposed, model.data());
            Core::checkGLError("ShapesAtlas::render glUniformMatrix4fv model");
            glDrawElements(shape->primitiveMode, shape->indicesCount, GL_UNSIGNED_INT, shape->dataIndicesOffset());
            Core::checkGLError("ShapesAtlas::render glDrawElements");
        }
    }

    void ShapesAtlas::setColor(const std::array<GLfloat, 4> &color)
    {
        glUniform4fv(colorLoc, Uniform4vColorCompCount, color.data());
        Core::checkGLError(name + "::setColor");
    }

    void ShapesAtlas::render(const shapeShPtr shape, const Matrix4 &model)
    {
        glUniformMatrix4fv(modelLoc, GLUniformMatrixCount, GLUniformMatrixTransposed, model.data());
        Core::checkGLError("ShapesAtlas::render glUniformMatrix4fv model");
        glDrawElements(shape->primitiveMode, shape->indicesCount, GL_UNSIGNED_INT, shape->dataIndicesOffset());
        Core::checkGLError("ShapesAtlas::render glDrawElements");
    }

} // namespace Core
