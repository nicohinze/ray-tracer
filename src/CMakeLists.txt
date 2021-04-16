cmake_minimum_required(VERSION 3.12)

set(TARGET ray-tracer)
set(SOURCE_FILES
    camera.cpp
    dielectric.cpp
    geometryobject.cpp
    intersection.cpp
    lambertian.cpp
    light.cpp
    metal.cpp
    ray.cpp
    raytracer.cpp
    sphere.cpp
    utils.cpp
)

add_library(${TARGET}-lib STATIC ${SOURCE_FILES})
target_compile_features(${TARGET}-lib PRIVATE cxx_std_17)
set_target_properties(${TARGET}-lib PROPERTIES CXX_EXTENSIONS OFF)
target_compile_options(${TARGET}-lib PRIVATE ${COMPILE_FLAGS})
target_link_options(${TARGET}-lib PRIVATE ${LINK_FLAGS})
find_package(Threads REQUIRED)
target_link_libraries(${TARGET}-lib PRIVATE Threads::Threads)
target_link_libraries(${TARGET}-lib PUBLIC glm)

add_executable(${TARGET} main.cpp)
target_link_libraries(${TARGET} PRIVATE ${TARGET}-lib)
target_compile_features(${TARGET} PRIVATE cxx_std_17)
set_target_properties(${TARGET} PROPERTIES CXX_EXTENSIONS OFF)
target_compile_options(${TARGET} PRIVATE ${COMPILE_FLAGS})
target_link_options(${TARGET} PRIVATE ${LINK_FLAGS})
