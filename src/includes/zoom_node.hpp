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

    public:
        ZoomNode(/* args */) = default;
        explicit ZoomNode(std::string name, Core::environmentShPtr environment, nodeWkPtr parent = std::weak_ptr<Node>{})
            : Core::Node(name, parent)
        {
            std::cout << "ZoomNode Node Constructor: '" << name << "'" << id << std::endl;
            env = environment;
        };
        ~ZoomNode() = default;

        int build(Core::NodeManager &nodeMan) override;

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

        AffineTransform &calcTransform();

        bool handleEvent(const IOEvent &event) override;
    };

} // namespace Core
