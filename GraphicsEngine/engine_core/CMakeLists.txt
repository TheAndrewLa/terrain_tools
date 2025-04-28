cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

project(ala_graphics_engine)

set(CMAKE_CXX_STANDARD 20 REQUIRED)
set(CMAKE_C_STANDARD 99 REQUIRED)

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
  boost_predef
  GIT_REPOSITORY https://github.com/boostorg/predef.git
  GIT_TAG boost-1.88.0
)

FetchContent_MakeAvailable(boost_predef)

add_library(boost_predef_interface INTERFACE)
target_include_directories(boost_predef_interface INTERFACE ${boost_predef_SOURCE_DIR}/include)

include_directories(include)
include_directories(glfw)
include_directories(boost_predef)

add_library(core STATIC)

target_include_directories(core PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${boost_SOURCE_DIR})
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(core PUBLIC glfw boost_predef_interface)

file(GLOB_RECURSE ENGINE_CORE_SOURCES src/*.cpp src/*.cc)
target_sources(core PRIVATE ${ENGINE_CORE_SOURCES})
