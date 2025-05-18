cmake_minimum_required(VERSION 3.31.0)

set(CMAKE_CXX_STANDARD           17)
# I want c++17 to the require minimum
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(COMMON_SOURCES
    src/app.cpp
    src/color4.cpp
)

# Add library as a ".a" file
add_library(CommonLib OBJECT
    ${COMMON_SOURCES}
)

# The relative paths are based on the location of the CMakeList file.
target_include_directories(CommonLib
    PUBLIC includes
    PUBLIC ../../src/includes
    PUBLIC ${GLAD_INCLUDES}
    PUBLIC ${glfw_SOURCE_DIR}/deps
    PUBLIC ${glfw_SOURCE_DIR}/include
)
