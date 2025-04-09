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

    std::cout << aft.toString4x4() << std::endl;
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

    cout << aft.toString4x4() << endl;

    Core::Vector2D p{1.0, 1.0};

    aft.transform(p);

    cout << p << endl;

    if (!(p.x == 2.0 && p.y == 3.0))
    {
        cout << "Expected p = <2.0,3.0> got: " << p << endl;
        std::exit(1);
    }

    cout << "======= End Test =========\n";
}

void makeScale_affinetransform()
{
    using std::cout;
    using std::endl;
    std::cout << "CTEST_FULL_OUTPUT" << std::endl; // <-- the fix

    cout << "---- TESTING: makeScale_affinetransform ----" << endl;

    Core::AffineTransform aft{};
    aft.makeScale(2.0, 3.0);

    cout << aft << endl;

    cout << aft.toString4x4() << endl;

    Core::Vector2D p{2.0, 2.0};

    aft.transform(p);

    cout << p << endl;

    if (!(p.x == 4.0 && p.y == 6.0))
    {
        cout << "Expected p = <4.0,6.0> got: " << p << endl;
        std::exit(1);
    }

    cout << "======= End Test =========\n";
}

void scale_affinetransform()
{
    using std::cout;
    using std::endl;
    std::cout << "CTEST_FULL_OUTPUT" << std::endl; // <-- the fix

    cout << "---- TESTING: scale_affinetransform ----" << endl;

    Core::AffineTransform aft{};
    aft.makeScale(2.0, 3.0);

    cout << aft << endl;

    cout << aft.toString4x4() << endl;

    Core::Vector2D p{2.0, 2.0};

    aft.transform(p);

    cout << p << endl;

    aft.scale(2.0, 2.0);

    aft.transform(p);

    cout << p << endl;

    if (!(p.x == 16.0 && p.y == 36.0))
    {
        cout << "Expected p = <16.0,36.0> got: " << p << endl;
        std::exit(1);
    }

    cout << "======= End Test =========\n";
}

void rotate_ccw_affinetransform()
{
    // OpenGL's +Y axis is downward
    //            -y
    //             |
    //             |
    //             | vector points in this quadrant
    // -x ------------------- +x
    //             |
    //             |
    //             |
    //            +y

    using std::cout;
    using std::endl;

    cout << "---- TESTING: rotate_ccw_affinetransform ----" << endl;

    Core::AffineTransform aft{};
    aft.makeRotate(-45.0 * Core::DEGREES_TO_RADIANS);

    cout << aft << endl;

    cout << aft.toString4x4() << endl;

    Core::Vector2D p{1.0, 0.0}; // +X axis

    aft.transform(p);

    cout << p << endl;

    if (epsilonCheck(0.70711, p.x))
    {
        checkShow("X component = ", 0.70711, 0.70711);
        std::exit(1);
    }

    if (epsilonCheck(-0.70711, p.y))
    {
        checkShow("Y component = ", -0.70711, p.x);
        std::exit(1);
    }

    cout << "======= End Test =========\n";
}
