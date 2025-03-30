cmake_minimum_required(VERSION 3.31.0)
project(ranger VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD           17)
# I want c++17 to the require minimum
set(CMAKE_CXX_STANDARD_REQUIRED YES)


project(ranger_beta CXX)


add_executable(unit_tests
    unit_tests.cpp
    vector_test.cpp
)

target_include_directories(unit_tests PRIVATE ../src/includes)

add_test(NAME create_zero_vector COMMAND unit_tests 1)