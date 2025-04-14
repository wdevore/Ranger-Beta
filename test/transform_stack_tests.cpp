#include <iostream>
// #include <catch2/catch_test_macros.hpp>

#include <transform_stack.hpp>
#include <matrix4.hpp>
#include <affinetransform.hpp>
#include <vector3.hpp>

void create_transform_stack()
{
    using std::cout;
    using std::endl;

    cout << "---- TESTING: create_transform_stack ----" << endl;

    Core::Matrix4 m{};
    m.setTranslate(1.5, 2.5, 0.0);

    Core::TransformStack stack{};
    stack.initialize(m);

    // 'current' should be set.
    cout << stack.current << endl;

    // Stack should be empty
    if (!stack.empty())
    {
        cout << "ERROR: Expected transform stack to be empty: " << endl;
        std::exit(1);
    }

    cout << "======= End Test =========" << endl;
}

void save_transform_stack()
{
    using std::cout;
    using std::endl;

    cout << "---- TESTING: save_transform_stack ----" << endl;

    Core::Matrix4 m{true};
    m.setTranslate(1.5, 2.5, 0.0);

    Core::TransformStack stack{};
    stack.initialize(m);

    // 'current' should be set.
    cout << stack.current << endl;

    stack.save();

    // Stack should NOT be empty
    if (stack.empty())
    {
        cout << "ERROR: Expected transform stack to NOT be empty: " << endl;
        std::exit(1);
    }

    cout << "======= End Test =========" << endl;
}

void apply_transform_stack()
{
    using std::cout;
    using std::endl;

    cout << "CTEST_FULL_OUTPUT" << endl; // <-- the fix

    cout << "---- TESTING: apply_transform_stack ----" << endl;

    Core::Matrix4 m{true};
    m.setTranslate(1.5, 2.5, 0.0);

    Core::TransformStack stack{};
    stack.initialize(m);

    // 'current' should be set.
    cout << stack.current << endl;

    stack.save();

    Core::AffineTransform aft{true};
    aft.makeTranslate(1.0, 1.0);

    stack.applyTransform(aft);

    cout << "After transform: " << stack.current << endl;

    Core::Vector3 v{};
    Core::Matrix4::getTranslation(stack.current, v);

    cout << v << endl;

    if (!(v.x == 2.5 && v.y == 3.5))
    {
        cout << "ERROR: Expected x,y = 2.5,3.5 ,Got: " << v << endl;
        std::exit(1);
    }

    cout << "======= End Test =========" << endl;
}

void restore_transform_stack()
{
    using std::cout;
    using std::endl;

    cout << "---- TESTING: restore_transform_stack ----" << endl;

    Core::Matrix4 m{true};
    m.setTranslate(1.5, 2.5, 0.0);

    Core::TransformStack stack{};
    stack.initialize(m);

    // 'current' should be set.
    cout << stack.current << endl;

    stack.save();

    Core::AffineTransform aft{true};
    aft.makeTranslate(1.0, 1.0);

    stack.applyTransform(aft);

    cout << "After transform: " << stack.current << endl;

    Core::Vector3 v{};
    Core::Matrix4::getTranslation(stack.current, v);

    cout << v << endl;

    if (!(v.x == 2.5 && v.y == 3.5))
    {
        cout << "ERROR: Expected x,y = 2.5,3.5 ,Got: " << v << endl;
        std::exit(1);
    }

    stack.restore();

    cout << "After restore: " << stack.current << endl;

    Core::Matrix4::getTranslation(stack.current, v);

    cout << v << endl;

    if (!(v.x == 1.5 && v.y == 2.5))
    {
        cout << "ERROR: Expected x,y = 1.5,2.5 ,Got: " << v << endl;
        std::exit(1);
    }

    cout << "======= End Test =========" << endl;
}