cmake_minimum_required(VERSION 3.16)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

include_directories(${CMAKE_SOURCE_DIR})

add_library(ExprTk INTERFACE)
target_include_directories(ExprTk INTERFACE "${CMAKE_SOURCE_DIR}/libs/ExprTk/")

add_subdirectory(viewer-core)
add_subdirectory(sviewer)
