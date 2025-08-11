cmake_minimum_required(VERSION 4.0)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set($ENV{CTEST_OUTPUT_ON_FAILURE} TRUE)
set(CTEST_OUTPUT_ON_FAILURE TRUE)
set(CTEST_VERBOSE TRUE)
set($ENV{CTEST_VERBOSE} ON)

project(cheech C)

add_compile_options(-fsanitize=address,undefined -g -Wall -Wextra -Werror)
add_link_options(
    -fsanitize=address,undefined
    -flto
    -Wl,-warn-common
    -Wl,-warn-unresolved-symbols)

add_executable(cheech "${CMAKE_SOURCE_DIR}/cheech/main.c")
target_include_directories(cheech PRIVATE "${CMAKE_SOURCE_DIR}")

include(cmake/common.cmake)
enable_testing()
add_c_test(lexer)
