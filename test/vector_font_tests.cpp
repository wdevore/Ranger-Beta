#include <iostream>
#include <memory>
#include <string>

#include <vector_font.hpp>

bool epsilonCheck(double expected, double actual);
void checkShow(const std::string &msg, double expected, double actual);

void create_vector_font()
{
    using std::cout;
    using std::endl;
    std::cout << "CTEST_FULL_OUTPUT" << std::endl; // <-- the fix

    cout << "---- TESTING: create_vector_font ----" << endl;

    Core::VectorFont font{};
    font.initialize();

    cout << "======= End Test =========\n";
}
