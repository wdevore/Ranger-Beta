#include <iostream>

#include <vector3.hpp>
#include <matrix4.hpp>
#include <constants.hpp>

bool epsilonCheck(double expected, double actual);
void checkShow(const std::string &msg, double expected, double actual);

void zero_matrix()
{
    std::cout << "---- TESTING: zero_matrix ----" << std::endl;
    Core::Matrix4 m{};

    std::cout << m << std::endl;

    std::cout << "======= End Test =========" << std::endl;
}

void identity_matrix()
{
    std::cout << "---- TESTING: identity_matrix ----" << std::endl;
    using M = Core::Matrix4;

    M m = M::createAsIdentity();

    std::cout << m << std::endl;

    std::cout << "======= End Test =========" << std::endl;
}

void set_translate_matrix()
{
    std::cout << "---- TESTING: set_translate_matrix ----" << std::endl;
    using M = Core::Matrix4;

    M m = M::createAsIdentity();

    m.setTranslate(1.0, 2.0, 3.0);

    std::cout << m << std::endl;

    auto e = m.e;

    if (!(e[M::m03] == 1.00 && e[M::m13] == 2.00 && e[M::m23] == 3.00))
    {
        std::cout << "ERROR: Expected matrix tx,ty,tz <1.0,2.0,3.0> got: " << m << std::endl;
        std::exit(1);
    }

    std::cout << "======= End Test =========" << std::endl;
}

void get_translate_matrix()
{
    std::cout << "---- TESTING: get_translate_matrix ----" << std::endl;
    using M = Core::Matrix4;

    M m = M::createAsIdentity();

    m.setTranslate(1.0, 2.0, 3.0);

    std::cout << m << std::endl;

    Core::Vector3 v{};

    M::getTranslation(m, v);

    if (!(v.x == 1.00 && v.y == 2.00 && v.z == 3.00))
    {
        std::cout << "ERROR: Expected matrix x,y,z <1.0,2.0,3.0> got: " << v << std::endl;
        std::exit(1);
    }

    std::cout << "======= End Test =========" << std::endl;
}

void set_rotation_matrix()
{
    // By simply adding the cout line below you force ctest to output regardless of
    // exceeding the threshold. This is the message you would get if you didn't:
    //
    // [This part of the test output was removed since it exceeds the threshold of 1024 bytes.]
    //
    std::cout << "CTEST_FULL_OUTPUT" << std::endl; // <-- the fix

    std::cout << "---- TESTING: set_rotation_matrix ----" << std::endl;
    using M = Core::Matrix4;

    M m = M::createAsIdentity();

    double rotAngle = 45.0 * Core::DEGREES_TO_RADIANS;

    m.setRotation(rotAngle);

    std::cout << m << std::endl;

    auto e = m.e;

    if (epsilonCheck(cos(rotAngle), e[M::m00]))
    {
        checkShow("m00 = ", cos(rotAngle), e[M::m00]);
        std::exit(1);
    }

    if (epsilonCheck(-sin(rotAngle), e[M::m01]))
    {
        checkShow("m01 = ", -sin(rotAngle), e[M::m01]);
        std::exit(1);
    }

    if (epsilonCheck(sin(rotAngle), e[M::m10]))
    {
        checkShow("m10 = ", sin(rotAngle), e[M::m10]);
        std::exit(1);
    }

    if (epsilonCheck(cos(rotAngle), e[M::m11]))
    {
        checkShow("m11 = ", cos(rotAngle), e[M::m11]);
        std::exit(1);
    }

    std::cout << "======= End Test =========" << std::endl;
}

void rotate_vector_ccw_matrix()
{
    // std::cout << "CTEST_FULL_OUTPUT" << std::endl; // <-- the fix

    //            +y
    //             |
    //             |
    //             | vector points in this quadrant
    // -x ------------------- +x
    //             |
    //             |
    //             |
    //            -y

    std::cout << "---- TESTING: rotate_vector_ccw_matrix ----" << std::endl;
    using M = Core::Matrix4;

    M m = M::createAsIdentity();

    double rotAngle = 25.0 * Core::DEGREES_TO_RADIANS;

    m.setRotation(rotAngle);

    std::cout << m << std::endl;

    Core::Vector3 xAxis{1.0, 0.0, 0.0};
    Core::Vector3 rotated{0.0, 0.0, 0.0};

    m.transformVector(xAxis, rotated);

    std::cout << "rotated vector: " << rotated << std::endl;

    if (epsilonCheck(cos(rotAngle), rotated.x))
    {
        checkShow("rotated.x = ", cos(rotAngle), rotated.x);
        std::exit(1);
    }

    if (epsilonCheck(sin(rotAngle), rotated.y))
    {
        checkShow("rotated.y = ", sin(rotAngle), rotated.y);
        std::exit(1);
    }

    std::cout << "======= End Test =========" << std::endl;
}

void rotate_vector_cw_matrix()
{
    // std::cout << "CTEST_FULL_OUTPUT" << std::endl; // <-- the fix

    //            +y
    //             |
    //             |
    //             |
    // -x ------------------- +x
    //             | vector points in this quadrant
    //             |
    //             |
    //            -y

    std::cout << "---- TESTING: rotate_vector_cw_matrix ----" << std::endl;
    using M = Core::Matrix4;

    M m = M::createAsIdentity();

    double rotAngle = -25.0 * Core::DEGREES_TO_RADIANS;

    m.setRotation(rotAngle);

    std::cout << m << std::endl;

    Core::Vector3 xAxis{1.0, 0.0, 0.0};
    Core::Vector3 rotated{0.0, 0.0, 0.0};

    m.transformVector(xAxis, rotated);

    std::cout << "rotated vector: " << rotated << std::endl;

    if (epsilonCheck(cos(rotAngle), rotated.x))
    {
        checkShow("rotated.x = ", cos(rotAngle), rotated.x);
        std::exit(1);
    }

    if (epsilonCheck(sin(rotAngle), rotated.y))
    {
        checkShow("rotated.y = ", -sin(rotAngle), rotated.y);
        std::exit(1);
    }

    std::cout << "======= End Test =========" << std::endl;
}

void translate_vector_point_matrix()
{
    // std::cout << "CTEST_FULL_OUTPUT" << std::endl; // <-- the fix

    std::cout << "---- TESTING: translate_vector_point_matrix ----" << std::endl;
    using M = Core::Matrix4;

    M m = M::createAsIdentity();

    // Set translation X axis only.
    // |   1.00000,   0.00000,   0.00000,   1.00000|
    // |   0.00000,   1.00000,   0.00000,   1.00000|
    // |   0.00000,   0.00000,   1.00000,   0.00000|
    // |   0.00000,   0.00000,   0.00000,   1.00000|
    m.setTranslate(1.0, 1.0, 0.0);

    std::cout << m << std::endl;

    Core::Vector3 v{1.0, 0.0, 0.0};
    Core::Vector3 transformed{0.0, 0.0, 0.0};

    m.transformVector(v, transformed);

    std::cout << "translated vector: " << transformed << std::endl;

    if (epsilonCheck(2.0, transformed.x))
    {
        checkShow("transformed.x = ", 2.0, transformed.x);
        std::exit(1);
    }

    if (epsilonCheck(1.0, transformed.y))
    {
        checkShow("transformed.x = ", 1.0, transformed.y);
        std::exit(1);
    }

    if (epsilonCheck(0.0, transformed.z))
    {
        checkShow("transformed.z = ", 0.0, transformed.z);
        std::exit(1);
    }

    std::cout << "======= End Test =========" << std::endl;
}

void scale_vector_matrix()
{
    // std::cout << "CTEST_FULL_OUTPUT" << std::endl; // <-- the fix

    std::cout << "---- TESTING: scale_vector_matrix ----" << std::endl;
    using M = Core::Matrix4;

    M m = M::createAsIdentity();

    // Set non-uniform scale
    m.setScale(3.0, 1.0);

    std::cout << m << std::endl;

    Core::Vector3 v{1.0, 2.0, 3.0};
    Core::Vector3 transformed{0.0, 0.0, 0.0};

    m.transformVector(v, transformed);

    std::cout << "scaled vector: " << transformed << std::endl;

    if (epsilonCheck(3.0, transformed.x))
    {
        checkShow("transformed.x = ", 3.0, transformed.x);
        std::exit(1);
    }

    if (epsilonCheck(2.0, transformed.y))
    {
        checkShow("transformed.x = ", 2.0, transformed.y);
        std::exit(1);
    }

    if (epsilonCheck(0.0, transformed.z))
    {
        checkShow("transformed.z = ", 0.0, transformed.z);
        std::exit(1);
    }

    std::cout << "======= End Test =========" << std::endl;
}