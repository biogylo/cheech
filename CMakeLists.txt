cmake_minimum_required(VERSION 4.0)
set(CMAKE_C_COMPILER clang)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set($ENV{CTEST_OUTPUT_ON_FAILURE} TRUE)
set(CTEST_OUTPUT_ON_FAILURE TRUE)
set(CTEST_VERBOSE TRUE)

set($ENV{CTEST_VERBOSE} ON)

project(cheech C)

add_compile_options(-fsanitize=address,undefined -g)
add_link_options(-fsanitize=address,undefined)

add_executable(cheech "${CMAKE_SOURCE_DIR}/cheech/main.c")
target_include_directories(cheech PRIVATE "${CMAKE_SOURCE_DIR}")

include(cmake/common.cmake)
enable_testing()
add_c_test(lexer)
