cmake_minimum_required(VERSION 3.31.0)

set(CMAKE_CXX_STANDARD           17)
# I want c++17 to the require minimum
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add library as a ".a" file
add_library(VectorsLib STATIC 
    shape.cpp
    dynamic_mono_atlas.cpp
    static_mono_atlas.cpp # deprecated: to be removed and replace with shapes atlas.
    shape_generator.cpp
    base_atlas.cpp
    vector_font_atlas.cpp
    shapes_atlas.cpp
    bitmap_vector_font_atlas.cpp
    vector_vector_font_atlas.cpp
    darkrose_bitmap_font.cpp
    particle_font_atlas.cpp
    )

target_include_directories(VectorsLib
    PUBLIC ../../../includes
    PUBLIC ${GLAD_INCLUDES}
    PUBLIC ${glfw_SOURCE_DIR}/deps
    PUBLIC ${glfw_SOURCE_DIR}/include
)

# Any other target that links against VectorsLib also needs RenderLib.
# The dependency is transitive.
target_link_libraries(VectorsLib
    PUBLIC GlobalDataLib
    PUBLIC GeometryLib
    PUBLIC ExtrasLib
    # Because headers of VectorsLib (.hpp files) include headers from RenderLib 
    # (like atlas.hpp), then PUBLIC is the correct choice.
    # VectorsLib only uses RenderLib in its implementation files (.cpp files) 
    # and not in its public headers, so we use PRIVATE.
    # Other targets that link against VectorsLib don't automatically get linked 
    # to RenderLib. This can help hide implementation details and can lead to 
    # faster link times in very large projects.
    PRIVATE RenderLib
)
