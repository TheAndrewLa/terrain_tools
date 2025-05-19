cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

project(ala_graphics_engine)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_CXX_STANDARD 20 REQUIRED)
set(CMAKE_C_STANDARD 99 REQUIRED)

add_subdirectory(engine_core)

add_executable(main main.cpp)
target_link_libraries(main core)
