#pragma once

#include <list>

#include "atlas.hpp"
#include "shape.hpp"
#include <shape_generator.hpp>

namespace Core
{
    class ShapesAtlas final : public Atlas
    {
    private:
        Core::ShapeGenerator generator{};

        std::list<shapeShPtr> shapes{};

        /// @brief This offset is byte offsets not integer numbers.
        /// It represents a "index pointer" where the next group of indices
        /// starts.
        int indicesByteOffset{0};

        // The atlas has shapes and each shape has vertices. These need to be
        // combined into a single array and later copied into GL Buffer.
        // At the same time each shape needs to be updated
        // to adjust element offsets and counts.
        int verticesBlockOffset{0};

        GLint colorLoc{};
        const std::string uniColor{"fragColor"};
        // Indicates only 1 color component
        const GLsizei Uniform4vColorCompCount{1};

        ErrorConditions initialAndBuildShader_();

        // Transfers the Shape to the backing collections and update offsets
        // for a potential additional shape.
        int transferShapeToBacking_(shapeShPtr shape);

    public:
        ShapesAtlas(/* args */);
        ~ShapesAtlas();

        ErrorConditions initialize();
        // Overload for l-values (makes a copy)
        const shapeShPtr addShape(const Shape &shape);

        // ----------------------------------------------------------------
        // ------------- Shape loader via generators ----------------------
        // ----------------------------------------------------------------
        int addSquare(ShapeControls alignment = ShapeControls::Centered, ShapeControls fillType = Core::ShapeControls::Filled);
        int addTriangle(ShapeControls alignment = ShapeControls::Centered, ShapeControls fillType = Core::ShapeControls::Filled);
        int addHLine();
        int addVLine();
        int addPlus();
        int addCircle(int segmentCount = 12, ShapeControls fillType = ShapeControls::Filled);
        int addArc(float startAngle = 0.0, float endAngle = DEGREES_TO_RADIANS * 45.0,
                   int segmentCount = 8, ShapeControls fillType = ShapeControls::Filled);

        /// @brief Call finalize when all geometry has been added.
        void finalize();

        ErrorConditions configureUniforms() override;
        const shapeShPtr getShapeById(const int id) const;

        void setColor(const std::array<GLfloat, 4> &color);
        void render(int shapeId, const Matrix4 &model);
        void render(const shapeShPtr shape, const Matrix4 &model);
    };

} // namespace Core
