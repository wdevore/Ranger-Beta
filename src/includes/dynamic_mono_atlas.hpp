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

    class DynamicMonoAtlas final : public BaseAtlas
    {
    private:
        /* data */
        ErrorConditions configure();
        ErrorConditions shake();
        /// @brief // Bake finalizes the Atlas by "baking" the shapes into the buffers.
        /// @return
        ErrorConditions bake();
        ErrorConditions configureUniforms();

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

        GLuint modelLoc{};
        GLuint colorLoc{};

    public:
        std::list<shapeShPtr> shapes{};
        int nextID{};

        Shape backingShape{};
        GLintptr vboBufferSize;

        // Buffers
        GLuint vaoID{};
        GLuint vboID{};
        GLuint eboID{};

        BasicShader shader{"mono_vertex.glsl", "mono_fragment.glsl"};

        DynamicMonoAtlas(/* args */) = default;
        ~DynamicMonoAtlas() = default;

        /// @brief Main method to call to load and compile.
        /// @return
        ErrorConditions burn();

        int addShape(std::string name,
                     const std::vector<GLfloat> &vertices,
                     std::vector<GLuint> &indices,
                     GLenum mode);

        shapeShPtr getShapeByName(const std::string &name) const;
        shapeShPtr getShapeById(const int id) const;

        void use();
        void unUse();

        void setColor(const std::array<GLfloat, 4> &color);
        void update();
        void copy();

        void setShapeVertex(double x, double y, int index, int shapeId);
        /// @brief SetVertex directly sets the backing buffer data.
        /// @param x
        /// @param y
        /// @param index
        void setVertex(double x, double y, int index);

        void render(int id, const Matrix4 &model);
    };

} // namespace Core
