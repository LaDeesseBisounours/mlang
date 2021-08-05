cmake_minimum_required(VERSION 3.21)

# Building using clang since we want to use llvm
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER clang)
set(CMAKE_FIND_LIBRARY_PREFIXES "lib")
set(CMAKE_FIND_LIBRARY_SUFFIXES ".so")

set( CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules)

include(FindCPPUNIT)

project(m_compiler VERSION 0.1)

add_subdirectory(src/)
include_directories(src/)

add_subdirectory(test/)

add_executable(mc "")
target_compile_options(mc PRIVATE  -Wall -Wextra -Werror -g)


target_sources(mc PUBLIC src/main.cpp)

target_link_libraries(mc framework)
