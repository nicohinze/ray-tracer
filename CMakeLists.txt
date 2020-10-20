cmake_minimum_required(VERSION 3.12)

project(
    ray-tracer
    VERSION 0.1
    DESCRIPTION "Some description"
    LANGUAGES CXX
)

option(RAY_TRACER_BUILD_TESTS "Build tests." ON)
option(RAY_TRACER_BUILD_CLANG_TIDY "Build with clang-tidy." OFF)
option(ENABLE_SANITIZER "Specify the enabled clang sanitizer")

if(ENABLE_SANITIZER STREQUAL "address")
    message(STATUS "Building with address sanitizer (ASAN)")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O1 -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O1 -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -g -O1 -fsanitize=address -fno-omit-frame-pointer")
elseif(ENABLE_SANITIZER STREQUAL "thread")
    message(STATUS "Building with thread sanitizer (TSAN)")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O1 -fsanitize=thread")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O1 -fsanitize=thread")
    set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -g -O1 -fsanitize=thread")
elseif(ENABLE_SANITIZER STREQUAL "undefined")
    message(STATUS "Building with undefined behavior sanitizer (UBSAN)")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -fsanitize=undefined")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fsanitize=undefined")
    set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -g -fsanitize=undefined")
endif()

add_subdirectory("external")
add_subdirectory("src")

include(CTest)
if(BUILD_TESTING AND RAY_TRACER_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
