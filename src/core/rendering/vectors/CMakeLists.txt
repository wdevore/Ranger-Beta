cmake_minimum_required(VERSION 3.31.0)

set(CMAKE_CXX_STANDARD           17)
# I want c++17 to the require minimum
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(VECTOR_SOURCES
    shape.cpp
    dynamic_mono_atlas.cpp
    static_mono_atlas.cpp
    bitmap_vector_font_atlas.cpp
    shape_generator.cpp
    base_atlas.cpp
)

# Add library as a ".a" file
add_library(VectorsLib OBJECT
    ${VECTOR_SOURCES}
)

target_include_directories(VectorsLib
    PUBLIC ../../../includes
    PUBLIC ${GLAD_INCLUDES}
    PUBLIC ${glfw_SOURCE_DIR}/deps
    PUBLIC ${glfw_SOURCE_DIR}/include
)

