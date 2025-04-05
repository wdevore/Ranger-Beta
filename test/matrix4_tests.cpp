#include <iostream>

#include <vector3.hpp>
#include <matrix4.hpp>

void zero_matrix()
{
    std::cout << "---- TESTING: zero matrix ----" << std::endl;
    Core::Matrix4 m{};

    std::cout << m << std::endl;

    std::cout << "======= End Test =========" << std::endl;
}

void identity_matrix()
{
    std::cout << "---- TESTING: identity matrix ----" << std::endl;
    using M = Core::Matrix4;

    M m = M::createAsIdentity();

    std::cout << m << std::endl;

    std::cout << "======= End Test =========" << std::endl;
}

void set_translate_matrix()
{
    std::cout << "---- TESTING: set translate matrix ----" << std::endl;
    using M = Core::Matrix4;

    M m = M::createAsIdentity();

    m.setTranslate(1.0, 2.0, 3.0);

    std::cout << m << std::endl;

    auto e = m.e;

    if (!(e[M::m03] == 1.00 && e[M::m13] == 2.00 && e[M::m23] == 3.00))
    {
        std::cout << "ERROR: matrix tx,ty,tz <1.0,2.0,3.0> got: " << m << std::endl;
        std::exit(1);
    }

    std::cout << "======= End Test =========" << std::endl;
}

void get_translate_matrix()
{
    std::cout << "---- TESTING: get translate matrix ----" << std::endl;
    using M = Core::Matrix4;

    M m = M::createAsIdentity();

    m.setTranslate(1.0, 2.0, 3.0);

    std::cout << m << std::endl;

    Core::Vector3 v{};

    M::getTranslation(m, v);

    auto e = m.e;

    if (!(v.x == 1.00 && v.y == 2.00 && v.z == 3.00))
    {
        std::cout << "ERROR: matrix x,y,z <1.0,2.0,3.0> got: " << v << std::endl;
        std::exit(1);
    }

    std::cout << "======= End Test =========" << std::endl;
}
