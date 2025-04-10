#include <iostream>
#include <memory>

#include <node.hpp>
#include <print_tree.hpp>

bool epsilonCheck(double expected, double actual);
void checkShow(const std::string &msg, double expected, double actual);

void create_node()
{
    using std::cout;
    using std::endl;

    cout << "---- TESTING: create_node ----" << endl;

    Core::NodeID = 0;
    Core::Node n{"test"};

    std::cout << "Node name: " << n.name << std::endl;

    std::cout << "======= End Test =========\n";
}

void append_child_node()
{
    using std::cout;
    using std::endl;

    cout << "---- TESTING: append_child_node ----" << endl;

    Core::NodeID = 0;
    Core::nodeShPtr boot = std::make_shared<Core::Node>("Boot");
    // Core::Node boot{"Boot"};
    std::cout << "Boot Node name: " << boot->name << std::endl;
    std::cout << "Boot Node id: " << boot->id << std::endl;

    Core::nodeShPtr splash = std::make_shared<Core::Node>("Splash");
    boot->appendChild(splash);

    Core::printTree(boot->getGroup().getChildren(), boot);

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
    Core::Node boot{"Boot"};
    std::cout << "Boot Node name: " << boot.name << std::endl;
    std::cout << "Boot Node id: " << boot.id << std::endl;

    Core::nodeShPtr splash = std::make_shared<Core::Node>("Splash");
    boot.appendChild(splash);

    Core::nodeShPtr intro = std::make_shared<Core::Node>("Intro");
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