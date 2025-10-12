cmake_minimum_required(VERSION 3.31.0)
project(simple_ubo VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD           17)
# I want c++17 to the require minimum
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "Current CMakeLists.txt directory: ${CMAKE_CURRENT_LIST_DIR}")

# add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/bin/core)

# ------------- Fetches
# Add Fetch feature
include(FetchContent) 

# ----------------- GLFW -------------------
# GLFW setup using FetchContent
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        3.4 # Or the latest stable tag
)
FetchContent_MakeAvailable(glfw)

# --------- Glad --------------------------
# We don't need to do this because I manually generated and downloaded the
# glad wrappers via: https://glad.dav1d.de/
# The settings are listed in glad.h

# This fetch only retrieves the generator code. You still need to use Cmake's
# COMMAND features to talk to the generator URL with custom
# parms and then request to download the zip. SEE README in this project.
# FetchContent_Declare(
#   glad
#   GIT_REPOSITORY https://github.com/Dav1dde/glad.git
#   GIT_TAG        v2.0.8 #Or latest release
# )
# FetchContent_MakeAvailable(glad)

# GLAD files
set(GLAD_SOURCES ../dependencies/glad/src)
set(GLAD_INCLUDES ../dependencies/glad/include)
message(STATUS "glad includes: " ${GLAD_INCLUDES})

set(SOURCE_DIR ../../src)

# message(STATUS "Compiling ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

add_executable(${PROJECT_NAME}
    main.cpp
    misc.cpp
    simple_ubo.cpp
    ${GLAD_SOURCES}/glad.c
)

# Required!!
# The "bin" value states that we'd like the binary output of src
# subdirectories to be placed in <build_tree>/bin. Otherwise, binary files would end up in
# <build_tree>/src, which could be confusing
# Also:
# It tells the linking system the paths to the libraries. In this
# we want to link against "CoreLib" which when built is in
# build/bin/core/libCoreLib.a
# "../../src" indicates the source dependencies if the library changes and
# "bin" is the root directory.
# Also:
# [source_dir]: This is the path to the subdirectory containing the CMakeLists.txt
# file you want CMake to process. This path is typically relative to the
# directory of the current CMakeLists.txt file.
# [binary_dir] (Optional): This is the path to the corresponding binary output 
# directory for the subdirectory. If omitted, CMake will create a binary 
# subdirectory with the same name as source_dir inside the current binary 
# directory. You can specify a different name or even a path relative to the 
# top-level binary directory for more complex build layouts. 
add_subdirectory(../../src bin)

message(STATUS "glfw dir: " ${glfw_SOURCE_DIR})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${SOURCE_DIR}/includes
    PUBLIC ${GLAD_INCLUDES}
    PUBLIC ${glfw_SOURCE_DIR}/deps
    PUBLIC ${glfw_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    glfw
    OpenGL
    CoreLib
)
