#include <iostream>
// #include <catch2/catch_test_macros.hpp>

#include <vector2d.hpp>
#include <constants.hpp>

bool epsilonCheck(double expected, double actual)
{
    double dif = fabs(expected - actual);
    return dif > Core::EPSILON;
}

void create_zero_vector()
{
    std::cout << "---- TESTING: zero vector ----" << std::endl;

    Core::Vector2D vector{};

    if (!(vector.x == 0 && vector.y == 0))
    {
        std::cout << "ERROR: vector not <0,0> got: " << vector << std::endl;
        std::exit(1);
    }

    std::cout << "======= End Test =========\n";
}

void vector_length()
{
    std::cout << "---- TESTING: vector length ----" << std::endl;
    Core::Vector2D vector{1.0, 1.0};

    double len = vector.getLength();
    std::cout << "vector len: " << len << std::endl;

    double dif = fabs(1.41421 - len);
    std::cout << "difference: " << dif << std::endl;

    if (epsilonCheck(1.41421, len))
    {
        std::cout << "ERROR: Expected vector len ~= to 1.41421 got: " << len << std::endl;
        std::exit(1);
    }
    std::cout << "======= End Test =========\n";
}

void vector_add()
{
    std::cout << "---- TESTING: vector add----" << std::endl;

    Core::Vector2D vector{};

    vector.add(1.0, 1.0);

    if (!(vector.x == 1 && vector.y == 1))
    {
        std::cout << "ERROR: Expected vector <1,1> got: " << vector << std::endl;
        // Sending 1 to exit() indicates the test failed.
        std::exit(1);
    }
    std::cout << "======= End Test =========\n";
}

void vector_sub()
{
    std::cout << "---- TESTING: vector sub----" << std::endl;

    Core::Vector2D vector{3.0, 4.0};
    Core::Vector2D vector2{1.0, 1.0};

    vector.sub(1.0, 1.0);

    if (!(vector.x == 2 && vector.y == 3))
    {
        std::cout << "ERROR: Expected vector <2,3> got: " << vector << std::endl;
        // Sending 1 to exit() indicates the test failed.
        std::exit(1);
    }

    vector.sub(vector2);

    if (!(vector.x == 1 && vector.y == 2))
    {
        std::cout << "ERROR: vector not <1,2> got: " << vector << std::endl;
        // Sending 1 to exit() indicates the test failed.
        std::exit(1);
    }

    std::cout << "======= End Test =========\n";
}

void vector_distance()
{
    std::cout << "---- TESTING: vector distance----" << std::endl;

    Core::Vector2D vector{1.0, 1.0};
    Core::Vector2D vector2{2.0, 2.0};

    double distance = vector.vectorDistance(vector, vector2);

    double dif = fabs(1.41421 - distance);
    std::cout << "difference: " << dif << std::endl;

    if (epsilonCheck(1.41421, distance))
    {
        std::cout << "ERROR: Expected vector distance 1.41421 got: " << distance << std::endl;
        // Sending 1 to exit() indicates the test failed.
        std::exit(1);
    }

    // two parallel lines 1.0 unit away from each other.
    vector.x = 1.0;
    vector.y = 0.0;
    vector2.x = 2.0;
    vector2.y = 0.0;
    distance = vector.vectorDistance(vector, vector2);
    std::cout << "distance: " << distance << std::endl;

    dif = fabs(1.0 - distance);
    std::cout << "difference: " << dif << std::endl;

    if (epsilonCheck(1.0, distance))
    {
        std::cout << "ERROR: Expected vector distance 1.0 got: " << distance << std::endl;
        // Sending 1 to exit() indicates the test failed.
        std::exit(1);
    }

    std::cout << "======= End Test =========\n";
}

void vector_normalize()
{
    std::cout << "---- TESTING: vector normalize----" << std::endl;

    Core::Vector2D vector{1.0, 1.0};

    vector.normalize();
    std::cout << "normalized: " << vector << std::endl;

    if (epsilonCheck(0.707107, vector.x))
    {
        std::cout << "ERROR: Expected vector.x value 0.707107 got: " << vector.x << std::endl;
        std::exit(1);
    }

    if (epsilonCheck(0.707107, vector.y))
    {
        std::cout << "ERROR: Expected vector.y value 0.707107 got: " << vector.x << std::endl;
        std::exit(1);
    }

    std::cout << "======= End Test =========\n";
}

void vector_angle_between()
{
    std::cout << "---- TESTING: vector angle between----" << std::endl;

    //            +Y
    //             |
    //             |  <\
    //   ----------o----\---- +X
    //             |
    //             |
    // Two vector 45 degrees apart = 0.785398 relative to
    // the origin. Counter clockwise (CCW)
    Core::Vector2D vector{1.0, 0.0};  // +X axis
    Core::Vector2D vector2{1.0, 1.0}; // +X/+Y diagonal 2nd quad

    double angle = vector.angleBetween(vector, vector2);
    std::cout << "angle: " << angle * Core::RADIANS_TO_DEGREES << std::endl;

    double dif = fabs(0.785398 - angle);
    std::cout << "difference: " << dif << std::endl;

    if (epsilonCheck(0.785398, angle))
    {
        std::cout << "ERROR: Expected vector angle 0.785398 got: " << angle << std::endl;
        // Sending 1 to exit() indicates the test failed.
        std::exit(1);
    }

    std::cout << "======= End Test =========\n";
}
// TEST_CASE("vector_add", "[vector]")
// {
//     Core::Vector2D vector{};

//     SECTION("Add constants to vector")
//     {
//         vector.add(1.0, 1.0);

//         REQUIRE(vector.x == 1.0);
//         REQUIRE(vector.y == 1.0);
//     }
// }
