cmake_minimum_required(VERSION 3.31.0)

set(CMAKE_CXX_STANDARD           17)
# I want c++17 to the require minimum
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(shaders)
add_subdirectory(vectors)

# This kind of a hack. I don't list any actual sources, but
# simply create a blank variable. I should use INTERFACEs but
# I can't get it to work.
set(RENDERING_SOURCES
    color.cpp
)

# Add library as a ".a" file
add_library(RenderLib STATIC
    ${RENDERING_SOURCES}
)

# RenderLib is built from these libraries
target_link_libraries(RenderLib
    PUBLIC ShadersLib
    PUBLIC VectorsLib
    # We need to include CoreLib because of deps on constants.cpp
    PUBLIC CoreLib
)
