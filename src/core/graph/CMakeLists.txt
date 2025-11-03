cmake_minimum_required(VERSION 3.31.0)

set(CMAKE_CXX_STANDARD           17)
# I want c++17 to the require minimum
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GRAPH_SOURCES
    group.cpp
    node.cpp
    transform.cpp
    transform_stack.cpp
    node_manager.cpp
)

# Add library as a ".a" file
add_library(GraphLib STATIC
    ${GRAPH_SOURCES}
)

target_include_directories(GraphLib
 PUBLIC ../../includes
)

target_link_libraries(GraphLib
    PUBLIC GlobalDataLib
    PUBLIC MathsLib
)
