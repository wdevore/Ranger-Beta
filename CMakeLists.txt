cmake_minimum_required(VERSION 3.31.0)
project(ranger VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD           17)
# I want c++17 to the require minimum
set(CMAKE_CXX_STANDARD_REQUIRED YES)

add_executable(ranger main.cpp)

# # If you don't include CTest you will get a "warning" that it
# # can't find DartConfiguration.tcl
include(CTest)
enable_testing()

# The "bin" value states that we'd like the binary output of src
# subdirectories to be placed in <build_tree>/bin. Otherwise, binary files would end up in
# <build_tree>/src, which could be confusing
add_subdirectory(src bin)

add_subdirectory(test)
