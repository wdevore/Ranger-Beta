cmake_minimum_required(VERSION 3.31.0)
# set(CMAKE_VERBOSE_MAKEFILE ON) # Enable verbose output during build

set(CMAKE_CXX_STANDARD           17)
# I want c++17 to the require minimum
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CORE_SOURCES
    engine.cpp
)

# Create our library. This is consumed by target_link_libraries command.
add_library(EngineLib OBJECT ${CORE_SOURCES})

# set_target_properties(EngineLib PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES)
# message(STATUS "CMAKE_CXX_STANDARD is set to: ${CMAKE_CXX_STANDARD}")

set(SOURCE_DIR core)

add_subdirectory(${SOURCE_DIR})


# I use INTERFACE because this is only used by dependencies.
target_include_directories(EngineLib
    INTERFACE includes
)

# CoreLib is built from these libraries
target_link_libraries(EngineLib
    PUBLIC CoreLib
)

# add_executable(main main.cpp)