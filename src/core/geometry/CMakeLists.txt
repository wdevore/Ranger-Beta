cmake_minimum_required(VERSION 3.31.0)

set(CMAKE_CXX_STANDARD           17)
# I want c++17 to the require minimum
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GEOM_SOURCES
    point.cpp
    rectangle.cpp
)

# Add library as a ".a" file
add_library(GeometryLib STATIC
    ${GEOM_SOURCES}
)

target_include_directories(GeometryLib
 PUBLIC ../../includes
)
