#pragma once

#include <iostream>
#include <string>
#include <memory>

#include "group.hpp"
#include "transform.hpp"
#include "affinetransform.hpp"
#include "transform_stack.hpp"
#include "io_event.hpp"

namespace Core
{
    enum class NodeSignal
    {
        /// A Node wants to leave stage
        requestNodeLeaveStage,

        /// A Node is given permission to leave
        leaveStageGranted,

        /// Notify a Node that it has moved to the Top
        nodeMovedToStage
    };

    std::ostream &operator<<(std::ostream &os, NodeSignal s);

    static int NodeID{0};

    using affineShPtr = std::shared_ptr<AffineTransform>;

    using nodeShPtr = std::shared_ptr<Node>;
    using nodeWkPtr = std::weak_ptr<Node>;

    class Environment;
    using environmentShPtr = std::shared_ptr<Environment>;

    class NodeManager;

    class Node
    {
    private:
        Group group_;
        Transform transform_;

    protected:
    public:
        int id{0};
        std::string name{};

        bool visible{true};
        bool dirty{true};

        nodeWkPtr parent;

        Node() = default;
        // This explicitly constructs a default-initialized std::weak_ptr,
        // which will be empty (i.e., not associated with any managed object,
        // similar to a null raw pointer). This is done because you can't
        // assign a default nullptr value to a weak_ptr.
        // If a shared_ptr is passed this it is converted to a weak_ptr via
        // a copy-constructor.
        Node(std::string name, nodeWkPtr parent = std::weak_ptr<Node>{})
            : name(name), parent(parent)
        {
            id = NodeID++;
            std::cout << "Node Constructor: '" << name << "'" << id << std::endl;
        };

        ~Node() = default;

        Group &getGroup();
        int getChildCount();
        std::list<nodeShPtr> getChildren();

        // Add to back
        void appendChild(nodeShPtr child);
        // Add to front
        void prependChild(nodeShPtr child);
        nodeShPtr findNode(int id);
        nodeShPtr findNode(const std::string &name);

        virtual int build(NodeManager &nodeMan) = 0;

        virtual void update(double dt);
        virtual bool handleEvent(const IOEvent &event);
        void visit(TransformStack &transformStack, double interpolation, double width, double height);

        void setPosition(float x, float y);
        Vector2D &getPosition();
        void setRotation(float radians);
        void setScale(float scale);
        void setScale(float scaleX, float scaleY);

        void rippleDirty(bool dirty);
        virtual AffineTransform &calcTransform();

        /// @brief
        /// render() provides a default render--which is to draw nothing.
        ///
        /// You should **override** this in your custom [Node] if your [Node]
        /// needs to perform custom rendering.
        virtual void render(const Matrix4 &model)
        {
            // std::cout << "Node::render" << std::endl;
        }

        virtual void receiveSignal(NodeSignal signal) = 0;

        // This is equivalent to toString()
        friend std::ostream &operator<<(std::ostream &os, const Node &obj)
        {
            os << ":'" << obj.name << "' (" << obj.id << "):";
            return os;
        }
    };

} // namespace Core
