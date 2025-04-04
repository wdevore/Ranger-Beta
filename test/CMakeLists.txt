cmake_minimum_required(VERSION 3.31.0)
project(unit_tests VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD           17)
# I want c++17 to the require minimum
set(CMAKE_CXX_STANDARD_REQUIRED YES)

# include(FetchContent)

# FetchContent_Declare(
#     Catch2
#     GIT_REPOSITORY https://github.com/catchorg/Catch2.git
#     GIT_TAG        v3.8.0 # Or the specific version you want to use
#     OVERRIDE_FIND_PACKAGE # Optional: If you have system-wide Catch2 installed, this will prioritize the fetched version
# )

# FetchContent_MakeAvailable(Catch2)

set(SOURCE_DIR ../src)

add_executable(${PROJECT_NAME}
    unit_tests.cpp
    vector_tests.cpp
    matrix4_tests.cpp
)

# target_link_libraries(unit_tests PRIVATE Catch2::Catch2)

# list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
# # If you don't include CTest you will get a "warning" that it
# # can't find DartConfiguration.tcl
include(CTest)
# include(Catch)

target_include_directories(${PROJECT_NAME} PRIVATE ${SOURCE_DIR}/includes)

# ----------------------- Test Suites ------------------------------------
add_test(NAME vector_tests COMMAND ${PROJECT_NAME} "vector_suite")
add_test(NAME matrix4_tests COMMAND ${PROJECT_NAME} "matrix_suite")

# catch_discover_tests(unit_tests)

# EngineLib is built from these libraries
target_link_libraries(${PROJECT_NAME}
    PUBLIC CoreLib
)
