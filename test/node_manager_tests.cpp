#include <iostream>
#include <memory>
#include <string>

#include <node.hpp>
#include <node_manager.hpp>

bool epsilonCheck(double expected, double actual);
void checkShow(const std::string &msg, double expected, double actual);

namespace TestNodes
{
    using nodeWkPtr = std::weak_ptr<Core::Node>;

    class BootNode : public Core::Node
    {
    private:
        /* data */
    public:
        bool gotSignal{false};

        BootNode(std::string name, nodeWkPtr parent = std::weak_ptr<Node>{})
            : Core::Node(name, parent)
        {
            std::cout << "BootNode Constructor: NM '" << name << "'" << id << std::endl;
        };
        ~BootNode() = default;

        void receiveSignal(Core::NodeSignal signal) override
        {
            gotSignal = true;
            std::cout << "BootNode Got signal: NM '" << signal << "' id: " << id << std::endl;
        }
    };

    class SplashNode : public Core::Node
    {
    private:
        /* data */
    public:
        bool gotSignal{false};

        SplashNode(std::string name, nodeWkPtr parent = std::weak_ptr<Node>{})
            : Node(name, parent)
        {
            std::cout << "SplashNode Constructor: NM  '" << name << "' id:" << id << std::endl;
        };
        ~SplashNode() = default;

        void receiveSignal(Core::NodeSignal signal) override
        {
            gotSignal = true;
            std::cout << "SplashNode Got signal: NM '" << signal << "' id:" << id << std::endl;
        }
    };

    class IntroNode : public Core::Node
    {
    private:
        /* data */
    public:
        bool gotSignal{false};

        IntroNode(std::string name, nodeWkPtr parent = std::weak_ptr<Node>{})
            : Node(name, parent)
        {
            std::cout << "IntroNode Constructor: NM  '" << name << "' id:" << id << std::endl;
        };
        ~IntroNode() = default;

        void receiveSignal(Core::NodeSignal signal) override
        {
            gotSignal = true;
            std::cout << "IntroNode Got signal: NM '" << signal << "' id:" << id << std::endl;
        }
    };

} // namespace Core

void create_node_manager()
{
    using std::cout;
    using std::endl;
    std::cout << "CTEST_FULL_OUTPUT" << std::endl; // <-- the fix

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

    Core::nodeShPtr boot = std::make_shared<TestNodes::BootNode>("Boot");

    nm.pushFront(boot);

    cout << "Nodes size: " << nm.nodes.size() << endl;

    if (nm.nodes.size() != 1)
    {
        cout << "Expected Node size of 1" << endl;
        std::exit(1);
    }

    bool onStage = nm.isNodeOnStage(boot);

    if (!onStage)
    {
        cout << "Expected Node on stage" << endl;
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

    Core::nodeShPtr boot = std::make_shared<TestNodes::BootNode>("Boot");

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

void signal_leave_stage_node_manager()
{
    using std::cout;
    using std::endl;

    cout << "---- TESTING: signal_leave_stage_node_manager ----" << endl;

    Core::NodeManager nm{};

    // Core::nodeShPtr boot =...  <-- can't use base Node because
    // it won't receive signals. You must use concrete Nodes.
    // 'auto' deduces correctly:
    // std::shared_ptr<TestNodes::BootNode> boot =...
    auto boot = std::make_shared<TestNodes::BootNode>("Boot");

    nm.pushFront(boot);

    cout << "Nodes size: " << nm.nodes.size() << endl;

    nm.sendSignal(boot, Core::NodeSignal::requestNodeLeaveStage);

    if (!boot->gotSignal)
    {
        cout << "Expected Boot to get signal" << endl;
        std::exit(1);
    }

    cout << "======= End Test =========" << endl;
}

void signal_to_stage_node_manager()
{
    using std::cout;
    using std::endl;

    cout << "---- TESTING: signal_to_stage_node_manager ----" << endl;

    Core::NodeManager nm{};

    auto boot = std::make_shared<TestNodes::BootNode>("Boot");
    auto splash = std::make_shared<TestNodes::SplashNode>("Splash");

    // These types of Nodes are considered Scenes meaning they are
    // not meant to be children, example calling boot->appendChild()
    nm.pushFront(boot);
    nm.pushBack(splash);

    cout << "Nodes size: " << nm.nodes.size() << endl;

    nm.sendSignal(boot, Core::NodeSignal::requestNodeLeaveStage);

    if (!boot->gotSignal)
    {
        cout << "Expected Boot to get signal" << endl;
        std::exit(1);
    }

    cout << "======= End Test =========" << endl;
}

void visit_node_manager()
{
    using std::cout;
    using std::endl;

    cout << "---- TESTING: visit_node_manager ----" << endl;

    Core::NodeManager nm{};

    auto boot = std::make_shared<TestNodes::BootNode>("Boot");
    auto splash = std::make_shared<TestNodes::SplashNode>("Splash");

    // These types of Nodes are considered Scenes meaning they are
    // not meant to be children (as in) calling boot->appendChild()
    nm.push(splash);
    nm.push(boot); // The last Node pushed is the one on Stage.

    cout << "Nodes size: " << nm.nodes.size() << endl;

    nm.visit(0.0, 100.0, 100.0);

    cout << "======= End Test =========" << endl;
}

void visit_pop_node_manager()
{
    using std::cout;
    using std::endl;

    cout << "---- TESTING: visit_node_manager ----" << endl;

    Core::NodeManager nm{};

    auto boot = std::make_shared<TestNodes::BootNode>("Boot");
    auto splash = std::make_shared<TestNodes::SplashNode>("Splash");

    // These types of Nodes are considered Scenes meaning they are
    // not meant to be children (as in) calling boot->appendChild()
    nm.push(splash);
    nm.push(boot); // The last Node pushed is the one on Stage.

    cout << "Nodes size: " << nm.nodes.size() << endl;

    nm.visit(0.0, 100.0, 100.0);

    nm.pop();

    nm.visit(0.0, 100.0, 100.0);

    cout << "======= End Test =========" << endl;
}

void children_node_manager()
{
    using std::cout;
    using std::endl;

    cout << "---- TESTING: visit_node_manager ----" << endl;

    Core::NodeManager nm{};

    auto boot = std::make_shared<TestNodes::BootNode>("Boot");
    auto splash = std::make_shared<TestNodes::SplashNode>("Splash");
    auto intro = std::make_shared<TestNodes::IntroNode>("Intro");
    splash->appendChild(intro);

    // These types of Nodes are considered Scenes meaning they are
    // not meant to be children (as in) calling boot->appendChild()
    nm.push(splash);
    nm.push(boot); // The last Node pushed is the one on Stage.

    cout << "Nodes size: " << nm.nodes.size() << endl;

    nm.visit(0.0, 100.0, 100.0);

    nm.pop();

    nm.visit(0.0, 100.0, 100.0);

    cout << "======= End Test =========" << endl;
}
