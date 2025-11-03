#pragma once
#include <memory>

#include "node.hpp"
#include "vector2d.hpp"
#include "zoom_transform.hpp"

namespace Core
{
    class ZoomNode final : public Node, public std::enable_shared_from_this<ZoomNode>
    {
    private:
        /* data */
        float zoomStepSize{};

        // State management
        float mx{};
        float my{};

        Vector2D zoomPoint{};
        ZoomTransform zoom{};
        int debug{};

        Matrix4 _viewSpace;

    public:
        ZoomNode(/* args */) = default;
        explicit ZoomNode(std::string name, nodeWkPtr parent = std::weak_ptr<Node>{})
            : Node(name, parent)
        {
            std::cout << "ZoomNode Node Constructor: '" << name << "'" << id << std::endl;
        };
        ~ZoomNode() = default;

        int build(Core::NodeManager &nodeMan) override;
        void setViewSpace(const Matrix4 &viewSpace) { _viewSpace = viewSpace; }

        /// @brief Sets the sensitivity of the zoom. If the view area
        /// is very tight then you want smaller values so that zooming
        /// doesn't jump by "glides"
        /// @param size
        void setStepSize(float size);
        /// @brief Sets the zooms position and ripples to children mouse is located.
        /// @param x
        /// @param y
        void setPostion(float x, float y);
        /// @brief SetFocalPoint sets the epi center of zoom
        /// @param x
        /// @param y
        void setFocalPoint(float x, float y);
        /// @brief sets the scale absolutely
        /// @param scale
        void scaleTo(float scale);
        /// @brief returns the zoom's current scale value
        /// @return
        float getZoomScale() const;
        /// @brief relative zooming using deltas
        /// @param dx
        /// @param dy
        void zoomBy(float dx, float dy);
        /// @brief is relative translation
        /// @param dx
        /// @param dy
        void translateBy(float dx, float dy);
        /// @brief zooms inward making things bigger
        void zoomIn();
        void zoomOut();

        /// @brief Zoom nodes manage their own transform differently.
        /// @return
        AffineTransform &calcTransform() override;

        bool handleEvent(const IOEvent &event) override;
        void receiveSignal(NodeSignal signal) override {}
    };

} // namespace Core
