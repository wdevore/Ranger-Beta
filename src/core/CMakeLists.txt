cmake_minimum_required(VERSION 3.31.0)
project(ranger VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD           17)
# I want c++17 to the require minimum
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(CORE_SOURCES
    maths/vector2d.cpp
)

# Add library as a ".o" file
add_library(CoreLib OBJECT
    ${CORE_SOURCES}
)

# I use INTERFACE because this is only used by dependencies.
target_include_directories(CoreLib
 INTERFACE ../includes
)
