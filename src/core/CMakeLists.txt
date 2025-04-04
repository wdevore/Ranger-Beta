cmake_minimum_required(VERSION 3.31.0)

set(CMAKE_CXX_STANDARD           17)
# I want c++17 to the require minimum
set(CMAKE_CXX_STANDARD_REQUIRED YES)

add_subdirectory(maths)

set(CORE_SOURCES
    maths/vector2d.cpp
    maths/matrix4.cpp
)

# Add library as a ".a" file
add_library(CoreLib OBJECT
    ${CORE_SOURCES}
)

# target_include_directories(${PROJECT_NAME}
#  PUBLIC ../includes
# )

# CoreLib is built from these libraries
target_link_libraries(CoreLib
    PUBLIC MathsLib
)
