cmake_minimum_required(VERSION 3.31.0)

set(CMAKE_CXX_STANDARD           17)
# I want c++17 to the require minimum
set(CMAKE_CXX_STANDARD_REQUIRED YES)

add_subdirectory(maths)
add_subdirectory(geometry)
add_subdirectory(graph)
add_subdirectory(extras)
add_subdirectory(rendering)

# This kind of a hack. I don't list any actual sources, but
# simply create a blank variable. I should use INTERFACEs but
# I can't get it to work.
set(CORE_SOURCES
    # Maths
    # maths/vector2d.cpp
    # maths/matrix4.cpp
    # maths/vector3.cpp
    # maths/affinetransform.cpp
    # Geometry
    # geometry/point.cpp
    # geometry/rectangle.cpp
    # Graph
    # graph/node.cpp
    # graph/group.cpp
    # graph/transform.cpp
    # graph/transform_stack.cpp
    # graph/node_manager.cpp
)

# Add library as a ".a" file
add_library(CoreLib STATIC
    ${CORE_SOURCES}
)

# target_include_directories(${PROJECT_NAME}
#  PUBLIC ../includes
# )

# CoreLib is built from these libraries
target_link_libraries(CoreLib
    PUBLIC MathsLib
    PUBLIC GeometryLib
    PUBLIC GraphLib
    PUBLIC ExtrasLib
    PUBLIC RenderLib
)
