cmake_minimum_required(VERSION 3.31.0)

set(CMAKE_CXX_STANDARD           17)
# I want c++17 to the require minimum
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GLOBAL_DATA_SOURCES
    global_data.cpp
)

# Add library as a ".a" file
add_library(GlobalDataLib STATIC
    ${GLOBAL_DATA_SOURCES}
)

target_include_directories(GlobalDataLib
    PUBLIC ../../includes
    PUBLIC ${GLAD_INCLUDES}
    PUBLIC ${glfw_SOURCE_DIR}/deps
    PUBLIC ${glfw_SOURCE_DIR}/include
)
