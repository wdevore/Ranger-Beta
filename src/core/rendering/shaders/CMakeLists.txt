cmake_minimum_required(VERSION 3.31.0)

set(CMAKE_CXX_STANDARD           17)
# I want c++17 to the require minimum
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(SHADER_SOURCES
    shader.cpp
    basic_shader.cpp
)

# Add library as a ".o" file
add_library(ShadersLib OBJECT
    ${SHADER_SOURCES}
)

target_include_directories(ShadersLib
 PUBLIC ../../../includes
 PUBLIC ${GLAD_INCLUDES}
 PUBLIC ${glfw_SOURCE_DIR}/deps
 PUBLIC ${glfw_SOURCE_DIR}/include
)
