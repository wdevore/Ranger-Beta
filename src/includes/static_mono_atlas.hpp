#pragma once

#include <list>
#include <array>

#include <glad/gl.h>

#include "shape.hpp"
#include "basic_shader.hpp"
#include "constants.hpp"
#include "base_atlas.hpp"

namespace Core
{
    class StaticMonoAtlas final : public BaseAtlas
    {
    private:
        /* data */
        ErrorConditions configure();
        ErrorConditions shake();
        /// @brief // Bake finalizes the Atlas by "baking" the shapes into the buffers.
        /// @return
        ErrorConditions bake();
        ErrorConditions configureUniforms();

        /// @brief This offset is byte offsets not integer numbers.
        /// It represents a "index pointer" where the next group of indices
        /// starts.
        int indicesByteOffset{0};
        // The atlas has shapes and each shape has vertices. These need to be
        // combined into a single array and later copied into GL Buffer.
        // At the same time each shape needs to be updated
        // to adjust element offsets and counts.
        int indiceBlockOffset{0};

        void vboBind(int bufferSize, const std::vector<GLfloat> &vertices);
        // Some may call this an "ibo" instead
        void eboBind(int bufferSize, const std::vector<GLuint> &indices);

        // ------- Uniforms ---------------
        // Fragment vars
        const std::string uniColor{"fragColor"};

        // Vertex vars
        const std::string uniModel{"model"}; // \x00
        const std::string uniProjection{"projection"};
        const std::string uniView{"view"};

        GLint modelLoc{};
        GLint colorLoc{};

    public:
        std::list<shapeShPtr> shapes{};
        // TODO std::unordered_map<std::string, VectorShapeSPtr> shapes{};

        int nextID{};

        // For the Shaking process
        Shape backingShape{};

        // Buffers
        GLuint vaoID{};
        GLuint vboID{};
        GLuint eboID{};

        BasicShader shader{"mono_vertex.glsl", "mono_fragment.glsl"};

        std::string lastError{};

        bool dirty{true};

        StaticMonoAtlas(/* args */) { std::cout << "StaticMonoAtlas ctor" << std::endl; };

        ~StaticMonoAtlas() = default;

        void initialize(environmentShPtr environment) override;

        /// @brief Main method to call to load and compile.
        /// @return
        ErrorConditions burn(bool andShake = false);

        /// @brief This only adds the shape to the collection. It doesn't shake it.
        /// @param shape
        /// @return
        int addShape(const Shape &shape);
        /// @brief Same as above.
        /// @param name
        /// @param vertices
        /// @param indices
        /// @param mode
        /// @return
        int addShape(std::string name,
                     const std::vector<GLfloat> &vertices,
                     std::vector<GLuint> &indices,
                     GLenum mode);

        int addShapeAndShake(Shape &shape);
        /// @brief Adds a shape dynamically to the atlas.
        /// @param shape
        /// @return
        int shakeShape(Shape &shape);

        shapeShPtr getShapeByName(const std::string &name) const;
        shapeShPtr getShapeById(const int id) const;
        int getIndicesOffset() const;

        void use();
        void unUse();

        void dispose();

        void setColor(const std::array<GLfloat, 4> &color);

        void render(int id, const Matrix4 &model);
        void render(const Matrix4 &model);
    };

    using staticAtlasShPtr = std::shared_ptr<StaticMonoAtlas>;
} // namespace Core
