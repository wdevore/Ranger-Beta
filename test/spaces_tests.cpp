#include <iostream>
// #include <catch2/catch_test_macros.hpp>

#include <worldbase.hpp>
#include <constants.hpp>

bool epsilonCheck(double expected, double actual);

void map_device_to_view_spaces()
{
    class World : public Core::Worldbase
    {
    private:
        /* data */
    public:
        World(double width, double height)
        {
            deviceSizeWidth = width;
            deviceSizeHeight = height;
        };
        ~World() = default;

        void construct()
        {
            std::cout << "World construct" << std::endl;
        }
    };

    std::cout << "---- TESTING: map_device_to_view_spaces ----" << std::endl;

    World world{1024, 800};

    if (!(world.deviceSizeWidth == 1024 && world.deviceSizeHeight == 800))
    {
        std::cout << "ERROR: World size not correct, got: " << world << std::endl;
        std::exit(1);
    }

    std::cout << "======= End Test =========\n";
}
