cmake_minimum_required(VERSION 3.31.0)
project(unit_tests VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD           17)
# I want c++17 to the require minimum
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(SOURCE_DIR ../src)

# add_subdirectory(${PROJECT_NAME} ${SOURCE_DIR}/src)

add_executable(${PROJECT_NAME}
    unit_tests.cpp
    vector_test.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${SOURCE_DIR}/includes)

add_test(NAME create_zero_vector COMMAND ${PROJECT_NAME} 1)

# EngineLib is built from these libraries
target_link_libraries(${PROJECT_NAME}
    PUBLIC CoreLib
)

# target_link_libraries(ranger
#     PUBLIC CoreLib
# )
