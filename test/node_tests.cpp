#include <iostream>
#include <memory>
#include <string>

#include <node.hpp>
#include <print_tree.hpp>

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
            std::cout << "BootNode Constructor: N '" << name << "'" << id << std::endl;
        };
        ~BootNode() = default;

        void receiveSignal(Core::NodeSignal signal) override
        {
            std::cout << "BootNode Got signal: N '" << signal << "'" << id << std::endl;
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
            std::cout << "SplashNode Constructor: N '" << name << "'" << id << std::endl;
        };
        ~SplashNode() = default;

        void receiveSignal(Core::NodeSignal signal) override
        {
            std::cout << "SplashNode Got signal: N '" << signal << "'" << id << std::endl;
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
            std::cout << "IntroNode Constructor: N '" << name << "'" << id << std::endl;
        };
        ~IntroNode() = default;

        void receiveSignal(Core::NodeSignal signal) override
        {
            std::cout << "IntroNode Got signal: N '" << signal << "'" << id << std::endl;
        }
    };

} // namespace Core

void create_node()
{
    using std::cout;
    using std::endl;

    cout << "---- TESTING: create_node ----" << endl;

    Core::NodeID = 0;
    Core::BootNode n{"test"};

    std::cout << "Node name: " << n.name << std::endl;

    std::cout << "======= End Test =========\n";
}

void append_child_node()
{
    using std::cout;
    using std::endl;

    cout << "---- TESTING: append_child_node ----" << endl;

    Core::NodeID = 0;
    Core::nodeShPtr boot = std::make_shared<Core::BootNode>("Boot");
    // Core::Node boot{"Boot"};
    std::cout << "Boot Node name: " << boot->name << std::endl;
    std::cout << "Boot Node id: " << boot->id << std::endl;

    Core::nodeShPtr splash = std::make_shared<Core::SplashNode>("Splash");
    boot->appendChild(splash);

    Core::printTree(boot->getChildren(), boot);

    if (splash.get()->name != "Splash")
    {
        cout << "Expected 'Splash' Got " << splash.get()->name << endl;
        std::exit(1);
    }

    if (splash.get()->id != 1)
    {
        cout << "Expected 'Splash id=1' Got " << splash.get()->id << endl;
        std::exit(1);
    }

    if (boot->id != 0)
    {
        cout << "Expected 'Boot id=0' Got " << boot->id << endl;
        std::exit(1);
    }

    if (boot->getChildCount() != 1)
    {
        cout << "Expected 1 child Got " << boot->getChildCount() << endl;
        std::exit(1);
    }
}

void find_node()
{
    using std::cout;
    using std::endl;

    cout << "---- TESTING: find_node_by_id_node ----" << endl;

    Core::NodeID = 0;
    Core::BootNode boot{"Boot"};
    std::cout << "Boot Node name: " << boot.name << std::endl;
    std::cout << "Boot Node id: " << boot.id << std::endl;

    Core::nodeShPtr splash = std::make_shared<Core::SplashNode>("Splash");
    boot.appendChild(splash);

    Core::nodeShPtr intro = std::make_shared<Core::IntroNode>("Intro");
    boot.appendChild(intro);

    Core::nodeShPtr node = boot.findNode(splash.get()->id);

    if (node == nullptr)
    {
        cout << "Expected to find Splash: Got nullptr" << endl;
        std::exit(1);
    }

    node = boot.findNode(3);

    if (node != nullptr)
    {
        cout << "Expected to NOT find id=3: Got non-nullptr" << endl;
        std::exit(1);
    }

    node = boot.findNode("Intro");

    if (node == nullptr)
    {
        cout << "Expected to find 'Intro' node: Got nullptr" << endl;
        std::exit(1);
    }

    node = boot.findNode("Exit");

    if (node != nullptr)
    {
        cout << "Expected to NOT find 'Exit' node" << endl;
        std::exit(1);
    }
}