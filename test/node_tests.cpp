#include <iostream>
#include <memory>

#include <node.hpp>

// #include <matrix4.hpp>
// #include <vector2d.hpp>
// #include <affinetransform.hpp>

// #include <constants.hpp>

bool epsilonCheck(double expected, double actual);
void checkShow(const std::string &msg, double expected, double actual);

void create_node()
{
    using std::cout;
    using std::endl;

    cout << "---- TESTING: create_node ----" << endl;

    Core::Node n{"test"};
    n.initialize();

    std::cout << "Node name: " << n.name << std::endl;

    std::cout << "======= End Test =========\n";
}

void append_child_node()
{
    using std::cout;
    using std::endl;

    cout << "---- TESTING: append_child_node ----" << endl;

    Core::Node boot{"Boot"};
    std::cout << "Boot Node name: " << boot.name << std::endl;
    boot.initialize();

    Core::nodeShPtr splash = std::make_shared<Core::Node>("Splash");
    boot.appendChild(splash);

    if (splash.get()->name != "Splash")
    {
        cout << "Expected 'Splash' Got " << splash.get()->name << endl;
        std::exit(1);
    }

    if (boot.getChildCount() != 1)
    {
        cout << "Expected 1 child Got " << boot.getChildCount() << endl;
        std::exit(1);
    }
}