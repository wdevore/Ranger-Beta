cmake_minimum_required(VERSION 3.31.0)

set(CMAKE_CXX_STANDARD           17)
# I want c++17 to the require minimum
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(GRAPH_SOURCES
    group.cpp
    node.cpp
    transform.cpp
)

# Add library as a ".a" file
add_library(GraphLib OBJECT
    ${GRAPH_SOURCES}
)

target_include_directories(GraphLib
 PUBLIC ../../includes
)
