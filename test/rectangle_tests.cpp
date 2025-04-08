#include <iostream>

#include <rectangle.hpp>
#include <vector2d.hpp>

#include <constants.hpp>

bool epsilonCheck(double expected, double actual);
void checkShow(const std::string &msg, double expected, double actual);

void point_contained_rectangle()
{
    std::cout << "---- TESTING: point_contained_rectangle ----" << std::endl;

    Core::Rectangle rec{};
    rec.set(100.0, 100.0);

    std::cout << rec << std::endl;

    Core::Vector2D p{25.0, 25.0};

    bool contained = rec.pointContained(p);

    if (!contained)
    {
        std::cout << "point not contained in rectangle" << std::endl;
        std::exit(1);
    }

    std::cout << "======= End Test =========\n";
}

void point_not_contained_rectangle()
{
    std::cout << "CTEST_FULL_OUTPUT" << std::endl; // <-- the fix

    std::cout << "---- TESTING: point_not_contained_rectangle ----" << std::endl;

    Core::Rectangle rec{};
    rec.set(100.0, 100.0);

    std::cout << rec << std::endl;

    Core::Vector2D p{125.0, 125.0};

    bool contained = rec.pointContained(p);

    if (contained)
    {
        std::cout << "point should not be contained in rectangle" << std::endl;
        std::exit(1);
    }

    std::cout << "======= End Test =========\n";
}

void point_inside_rectangle()
{
    std::cout << "---- TESTING: point_inside_rectangle ----" << std::endl;

    Core::Rectangle rec{};
    rec.set(100.0, 100.0);

    std::cout << rec << std::endl;

    Core::Vector2D p{0.0, 25.0};

    bool contained = rec.pointInside(p);

    if (!contained)
    {
        std::cout << "point should be contained in rectangle" << std::endl;
        std::exit(1);
    }

    std::cout << "======= End Test =========\n";
}

void point_inside_right_edge_rectangle()
{
    std::cout << "---- TESTING: point_inside_right_edge_rectangle ----" << std::endl;

    Core::Rectangle rec{};
    rec.set(100.0, 100.0);

    std::cout << rec << std::endl;

    // Y straddles bottom edge which means it is outside
    Core::Vector2D p{0.0, 100.0};

    bool contained = rec.pointInside(p);

    if (contained)
    {
        std::cout << "point should not be contained in rectangle" << std::endl;
        std::exit(1);
    }

    std::cout << "======= End Test =========\n";
}

void rectangle_intersects_rectangle()
{
    std::cout << "---- TESTING: rectangle_intersects_rectangle ----" << std::endl;

    Core::Rectangle rec{};
    rec.set(100.0, 100.0);

    std::cout << rec << std::endl;

    Core::Rectangle rec2{};
    rec2.set(75.0, 75.0, 150.0, 150.0);

    std::cout << rec2 << std::endl;

    // They should intersect
    bool intersected = rec.intersects(rec2);

    if (!intersected)
    {
        std::cout << "rectangle should intersect rectangle" << std::endl;
        std::exit(1);
    }

    std::cout << "======= End Test =========\n";
}

void rectangle_not_intersects_rectangle()
{
    std::cout << "---- TESTING: rectangle_not_intersects_rectangle ----" << std::endl;

    Core::Rectangle rec{};
    rec.set(100.0, 100.0);

    std::cout << rec << std::endl;

    Core::Rectangle rec2{};
    rec2.set(175.0, 175.0, 150.0, 150.0);

    std::cout << rec2 << std::endl;

    // They should intersect
    bool intersected = rec.intersects(rec2);

    if (intersected)
    {
        std::cout << "rectangle should not intersect rectangle" << std::endl;
        std::exit(1);
    }

    std::cout << "======= End Test =========\n";
}

void rectangle_contains_rectangle()
{
    std::cout << "---- TESTING: rectangle_contains_rectangle ----" << std::endl;

    Core::Rectangle rec{};
    rec.set(100.0, 100.0);

    std::cout << rec << std::endl;

    Core::Rectangle rec2{};
    rec2.set(25.0, 25.0, 50.0, 50.0);

    std::cout << rec2 << std::endl;

    // They rec should contain rec2
    bool contains = rec.contains(rec2);

    if (!contains)
    {
        std::cout << "'rec' should contain 'rec2' rectangle" << std::endl;
        std::exit(1);
    }

    std::cout << "======= End Test =========\n";
}

void rectangle_not_contains_rectangle()
{
    std::cout << "---- TESTING: rectangle_not_contains_rectangle ----" << std::endl;

    Core::Rectangle rec{};
    rec.set(100.0, 100.0);

    std::cout << rec << std::endl;

    Core::Rectangle rec2{};
    rec2.set(90.0, 90.0, 150.0, 150.0);

    std::cout << rec2 << std::endl;

    // They rec should NOT contain rec2
    bool contains = rec.contains(rec2);

    if (contains)
    {
        std::cout << "'rec' should not contain 'rec2' rectangle" << std::endl;
        std::exit(1);
    }

    std::cout << "======= End Test =========\n";
}
