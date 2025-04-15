cmake_minimum_required(VERSION 3.31.0)

set(CORE_SOURCES
    engine.cpp
)

# Create our library. This is consumed by target_link_libraries command.
add_library(EngineLib OBJECT ${CORE_SOURCES})

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