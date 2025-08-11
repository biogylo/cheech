cmake_minimum_required(VERSION 4.0)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set($ENV{CTEST_OUTPUT_ON_FAILURE} 1)
set(CTEST_OUTPUT_ON_FAILURE 1)
set(CTEST_VERBOSE TRUE)
# set(CPPLINT_COMMAND_ARGS "--linelength=120")

set(CMAKE_C_CPPCHECK cppcheck)#;--enable=all;--error-exitcode=1)
set(CMAKE_C_CPPLINT cpplint;--linelength=120)
set($ENV{CTEST_VERBOSE} ON)
set(CMAKE_C_FLAGS_RELEASE "-O3")

project(cheech C)

add_compile_options(
    -fsanitize=address,undefined
    -g
    -Wall
    -Wextra
    -Wpedantic
    -Werror
)

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
