cmake_minimum_required(VERSION 3.31.0)

set(CMAKE_CXX_STANDARD           17)
# I want c++17 to the require minimum
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(EXTRAS_SOURCES
    vector_font.cpp
)

# Add library as a ".a" file
add_library(ExtrasLib OBJECT
    ${EXTRAS_SOURCES}
)

target_include_directories(ExtrasLib
 PUBLIC ../../includes
)
