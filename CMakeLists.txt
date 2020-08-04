cmake_minimum_required(VERSION 3.12)

project(
    ray-tracer
    VERSION 0.1
    DESCRIPTION "Some description"
    LANGUAGES CXX
)

option(RAY_TRACER_BUILD_TESTS "Build tests." ON)

# set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-header-filter=.;-checks=*,-cppcoreguidelines-avoid-magic-numbers,-modernize-use-nodiscard;-warnings-as-errors=*")

add_subdirectory("external/catch")
add_subdirectory("src")

include(CTest)
if(BUILD_TESTING AND RAY_TRACER_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
