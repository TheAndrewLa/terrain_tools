cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

project(ala_graphics_engine)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_CXX_STANDARD 20 REQUIRED)
set(CMAKE_C_STANDARD 99 REQUIRED)

include(FetchContent)

FetchContent_Declare(
    boost_predef
    GIT_REPOSITORY https://github.com/boostorg/predef.git
    GIT_TAG boost-1.87.0
)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

FetchContent_MakeAvailable(glfw)
FetchContent_MakeAvailable(boost_predef)

add_subdirectory(engine_core)

add_executable(main main.cpp)
target_link_libraries(main core)
