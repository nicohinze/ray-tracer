cmake_minimum_required(VERSION 3.12)

set(
    TEST_FILES
    ray_utest.cpp
)

foreach(FILE ${TEST_FILES})
    get_filename_component(FILE_BASENAME ${FILE} NAME_WE)
    string(REGEX REPLACE "^(.+)_utest" "test-\\1" TEST_CASE ${FILE_BASENAME})
    add_executable(${TEST_CASE} ${FILE})
    target_link_libraries(${TEST_CASE} PRIVATE catch ray-tracer-lib)
    target_include_directories(${TEST_CASE} PRIVATE "${CMAKE_SOURCE_DIR}/src")
    add_test(NAME ${TEST_CASE} COMMAND ${TEST_CASE})
    set_tests_properties(${TEST_CASE} PROPERTIES LABELS "ray-tracer-unit-tests")
endforeach()
