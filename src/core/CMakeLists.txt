cmake_minimum_required(VERSION 3.31.0)

set(CMAKE_CXX_STANDARD           17)
# I want c++17 to the require minimum
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(maths)
add_subdirectory(geometry)
add_subdirectory(graph)
add_subdirectory(extras)
add_subdirectory(rendering)
add_subdirectory(global_data)


# This kind of a hack. I don't list any actual sources, but
# simply create a blank variable. I should use INTERFACEs but
# I can't get it to work.
# Update, I decided to move constants here
set(CORE_SOURCES
    io_event.cpp
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
    PUBLIC GlobalDataLib
)
