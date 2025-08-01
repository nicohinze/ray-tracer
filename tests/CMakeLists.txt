cmake_minimum_required(VERSION 3.12)

include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.8.1
)
FetchContent_MakeAvailable(Catch2)

set(TEST_FILES camera_unit_test.cpp image_texture_unit_test.cpp ray_unit_test.cpp sphere_unit_test.cpp)

foreach(file_name ${TEST_FILES})
    get_filename_component(file_basename ${file_name} NAME_WE)
    string(REGEX REPLACE "^(.+)_unit_test" "test-\\1" TEST_CASE ${file_basename})
    add_executable(${TEST_CASE} ${file_name})
    target_compile_features(${TEST_CASE} PRIVATE cxx_std_20)
    set_target_properties(${TEST_CASE} PROPERTIES CXX_EXTENSIONS OFF)
    target_compile_options(${TEST_CASE} PRIVATE ${COMPILE_FLAGS})
    target_link_options(${TEST_CASE} PRIVATE ${LINK_FLAGS})
    target_link_libraries(${TEST_CASE} PRIVATE Catch2::Catch2WithMain ray-tracer-lib)
    target_include_directories(${TEST_CASE} PRIVATE "${PROJECT_SOURCE_DIR}/src")
    add_test(NAME ${TEST_CASE} COMMAND ${TEST_CASE})
    set_tests_properties(${TEST_CASE} PROPERTIES LABELS "ray-tracer-unit-tests")
endforeach()
