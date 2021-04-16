cmake_minimum_required(VERSION 3.12)

set(
    TEST_FILES
    camera_unit_test.cpp
    ray_unit_test.cpp
    sphere_unit_test.cpp
)

foreach(FILE ${TEST_FILES})
    get_filename_component(FILE_BASENAME ${FILE} NAME_WE)
    string(REGEX REPLACE "^(.+)_unit_test" "test-\\1" TEST_CASE ${FILE_BASENAME})
    add_executable(${TEST_CASE} ${FILE})
    target_compile_features(${TEST_CASE} PRIVATE cxx_std_17)
    set_target_properties(${TEST_CASE} PROPERTIES CXX_EXTENSIONS OFF)
    target_compile_options(${TEST_CASE} PRIVATE ${COMPILE_FLAGS})
    target_link_options(${TEST_CASE} PRIVATE ${LINK_FLAGS})
    target_link_libraries(${TEST_CASE} PRIVATE catch ray-tracer-lib)
    target_include_directories(${TEST_CASE} PRIVATE "${PROJECT_SOURCE_DIR}/src")
    add_test(NAME ${TEST_CASE} COMMAND ${TEST_CASE})
    set_tests_properties(${TEST_CASE} PROPERTIES LABELS "ray-tracer-unit-tests")
endforeach()
