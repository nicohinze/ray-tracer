cmake_minimum_required(VERSION 3.12)

project(
    ray-tracer
    VERSION 0.1
    DESCRIPTION "Some description"
    LANGUAGES CXX
)

option(RAY_TRACER_BUILD_TESTS "Build tests." ON)
option(RAY_TRACER_BUILD_COVERAGE "Build with coverage information." OFF)
option(RAY_TRACER_BUILD_CLANG_TIDY "Build with clang-tidy." OFF)
option(RAY_TRACER_ENABLE_SANITIZER "Specify the enabled clang sanitizer")

add_subdirectory("external")

if(RAY_TRACER_BUILD_CLANG_TIDY)
    message(STATUS "✅ RAY_TRACER_BUILD_CLANG_TIDY is enabled")
    set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-header-filter=^(src|tests)/.*")
else()
    message(STATUS "❌ RAY_TRACER_BUILD_CLANG_TIDY is disabled")
endif()

if(RAY_TRACER_ENABLE_SANITIZER STREQUAL "address")
    message(STATUS "✅ RAY_TRACER_ENABLE_SANITIZER is set to '${RAY_TRACER_ENABLE_SANITIZER}'")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O1 -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O1 -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -g -O1 -fsanitize=address -fno-omit-frame-pointer")
elseif(RAY_TRACER_ENABLE_SANITIZER STREQUAL "thread")
    message(STATUS "✅ RAY_TRACER_ENABLE_SANITIZER is set to '${RAY_TRACER_ENABLE_SANITIZER}'")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O1 -fsanitize=thread")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O1 -fsanitize=thread")
    set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -g -O1 -fsanitize=thread")
elseif(RAY_TRACER_ENABLE_SANITIZER STREQUAL "undefined")
    message(STATUS "✅ RAY_TRACER_ENABLE_SANITIZER is set to '${RAY_TRACER_ENABLE_SANITIZER}'")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -fsanitize=undefined")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fsanitize=undefined")
    set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -g -fsanitize=undefined")
else()
    message(STATUS "❌ RAY_TRACER_ENABLE_SANITIZER is not set")
endif()

set(COMPILE_FLAGS -Wall -Werror -Wextra -Wpedantic)

if(RAY_TRACER_BUILD_TESTS AND RAY_TRACER_BUILD_COVERAGE)
    message(STATUS "✅ RAY_TRACER_BUILD_COVERAGE is enabled")
    set(COMPILE_FLAGS "${COMPILE_FLAGS}" --coverage -g -O0)
    set(LINK_FLAGS --coverage)

    add_custom_target(
        coverage_report
        COMMENT "Generating coverage report"
        COMMAND lcov --capture --directory . --output-file coverage.info --rc branch_coverage=1 --ignore-errors mismatch,inconsistent
        COMMAND lcov --extract coverage.info "*/${CMAKE_PROJECT_NAME}/src/*" --output-file coverage.info.filtered --rc branch_coverage=1
        COMMAND genhtml --title "${CMAKE_PROJECT_NAME}" --legend --demangle-cpp --output-directory coverage_report
                --show-details --branch-coverage coverage.info.filtered
    )
else()
    message(STATUS "❌ RAY_TRACER_BUILD_COVERAGE is disabled")
endif()

add_subdirectory("src")

include(CTest)
if(BUILD_TESTING AND RAY_TRACER_BUILD_TESTS)
    message(STATUS "✅ RAY_TRACER_BUILD_TESTS is enabled")
    enable_testing()
    add_subdirectory(tests)
else()
    message(STATUS "❌ RAY_TRACER_BUILD_TESTS is disabled")
endif()
