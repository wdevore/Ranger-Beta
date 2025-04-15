#include <iostream>
#include <memory>
#include <string>

#include <node.hpp>
#include <node_manager.hpp>

bool epsilonCheck(double expected, double actual);
void checkShow(const std::string &msg, double expected, double actual);

namespace Core
{
    using nodeWkPtr = std::weak_ptr<Node>;

    class BootNode : public Node
    {
    private:
        /* data */
    public:
        BootNode(std::string name, nodeWkPtr parent = std::weak_ptr<Node>{})
            : Node(name, parent)
        {
            std::cout << "BootNode Constructor: '" << name << "'" << id << std::endl;
        };
        ~BootNode() = default;

        void receiveSignal(Core::NodeSignal signal) override
        {
        }
    };

    class SplashNode : public Node
    {
    private:
        /* data */
    public:
        SplashNode(std::string name, nodeWkPtr parent = std::weak_ptr<Node>{})
            : Node(name, parent)
        {
            std::cout << "SplashNode Constructor: '" << name << "'" << id << std::endl;
        };
        ~SplashNode() = default;

        void receiveSignal(Core::NodeSignal signal) override
        {
        }
    };

    class IntroNode : public Node
    {
    private:
        /* data */
    public:
        IntroNode(std::string name, nodeWkPtr parent = std::weak_ptr<Node>{})
            : Node(name, parent)
        {
            std::cout << "IntroNode Constructor: '" << name << "'" << id << std::endl;
        };
        ~IntroNode() = default;

        void receiveSignal(Core::NodeSignal signal) override
        {
        }
    };

} // namespace Core

void create_node_manager()
{
    using std::cout;
    using std::endl;

    cout << "---- TESTING: create_node_manager ----" << endl;

    Core::NodeManager nm{};

    cout << "======= End Test =========\n";
}

void push_node_manager()
{
    using std::cout;
    using std::endl;

    cout << "---- TESTING: push_node_manager ----" << endl;

    Core::NodeManager nm{};

    Core::nodeShPtr boot = std::make_shared<Core::BootNode>("Boot");

    nm.pushFront(boot);

    cout << "Nodes size: " << nm.nodes.size() << endl;

    if (nm.nodes.size() != 1)
    {
        cout << "Expected Node size of 1" << endl;
        std::exit(1);
    }

    cout << "======= End Test =========" << endl;
}

void remove_node_manager()
{
    using std::cout;
    using std::endl;

    cout << "---- TESTING: remove_node_manager ----" << endl;

    Core::NodeManager nm{};

    Core::nodeShPtr boot = std::make_shared<Core::BootNode>("Boot");

    nm.pushFront(boot);

    cout << "Nodes size: " << nm.nodes.size() << endl;

    nm.removeNode("Boot");

    cout << "Nodes size: " << nm.nodes.size() << endl;

    if (nm.nodes.size() != 0)
    {
        cout << "Expected Node size of 0" << endl;
        std::exit(1);
    }

    cout << "======= End Test =========" << endl;
}
