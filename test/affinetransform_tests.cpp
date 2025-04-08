#include <iostream>

#include <matrix4.hpp>
#include <vector2d.hpp>
#include <affinetransform.hpp>

#include <constants.hpp>

bool epsilonCheck(double expected, double actual);
void checkShow(const std::string &msg, double expected, double actual);

void set_affinetransform()
{
    std::cout << "---- TESTING: set_affinetransform ----" << std::endl;

    Core::AffineTransform aft{};
    aft.set(1.0, 2.0, 3.0, 4.0, 5.0, 6.0);

    std::cout << aft << std::endl;

    std::cout << aft.toString4x4().str() << std::endl;
    // Core::Vector2D p{25.0, 25.0};

    // bool contained = rec.pointContained(p);

    // if (!contained)
    // {
    //     std::cout << "point not contained in rectangle" << std::endl;
    //     std::exit(1);
    // }

    std::cout << "======= End Test =========\n";
}

void maketranslate_affinetransform()
{
    using std::cout;
    using std::endl;

    cout << "---- TESTING: maketranslate_affinetransform ----" << endl;

    Core::AffineTransform aft{};
    aft.makeTranslate(1.0, 2.0);

    cout << aft << endl;

    cout << aft.toString4x4().str() << endl;

    Core::Vector2D p{1.0, 1.0};

    aft.transform(p);

    cout << p << endl;

    if (!(p.x == 2.0 && p.y == 3.0))
    {
        cout << "Expected p = <2.0,3.0> got: " << p << endl;
        std::exit(1);
    }

    std::cout << "======= End Test =========\n";
}
