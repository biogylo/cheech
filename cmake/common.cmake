# Creates a unit test
# Assume that all tests will have a file called `test_<name>`
function(add_c_test name)
    set(target "test_${name}")
    add_executable(
        ${target} 
        "${CMAKE_SOURCE_DIR}/test/${target}.c"
        "${CMAKE_SOURCE_DIR}/cheech/asan.c"
    )
    target_include_directories(${target} PRIVATE "${CMAKE_SOURCE_DIR}")
    add_test(NAME ${name} COMMAND ${target})
endfunction()
