cmake_minimum_required(VERSION 3.31.0)

project(r VERSION 0.1.0 LANGUAGES C CXX)

set(CORE_SOURCES
    main.cpp
)
add_library(CoreLib STATIC ${CORE_SOURCES})

# add_executable(main main.cpp)