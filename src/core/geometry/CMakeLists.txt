cmake_minimum_required(VERSION 3.31.0)

set(CMAKE_CXX_STANDARD           17)
# I want c++17 to the require minimum
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(MATHS_SOURCES
    point.cpp
    rectangle.cpp
)

# Add library as a ".a" file
add_library(GeometryLib OBJECT
    ${MATHS_SOURCES}
)

# # I use INTERFACE because this is only used by dependencies.
target_include_directories(GeometryLib
 PUBLIC ../../includes
)
