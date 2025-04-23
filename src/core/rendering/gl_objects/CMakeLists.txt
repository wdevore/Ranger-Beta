cmake_minimum_required(VERSION 3.31.0)

set(CMAKE_CXX_STANDARD           17)
# I want c++17 to the require minimum
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(GLOBJECTS_SOURCES
    vao.cpp
    vbo.cpp
    ebo.cpp
    mesh.cpp
)

# Add library as a ".a" file
add_library(GLObjectsLib OBJECT
    ${GLOBJECTS_SOURCES}
)

target_include_directories(GLObjectsLib
 PUBLIC ../../../includes
 PUBLIC ${GLAD_INCLUDES}
 PUBLIC ${glfw_SOURCE_DIR}/deps
 PUBLIC ${glfw_SOURCE_DIR}/include
)
