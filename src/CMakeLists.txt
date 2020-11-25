cmake_minimum_required(VERSION 3.12)

set(
    SOURCE_FILES
    camera.cpp
    geometryobject.cpp
    intersection.cpp
    light.cpp
    material.cpp
    ray.cpp
    raytracer.cpp
    sphere.cpp
    utils.cpp
)

add_library(ray-tracer-lib STATIC ${SOURCE_FILES})
target_compile_features(ray-tracer-lib PRIVATE cxx_std_17)
set_target_properties(ray-tracer-lib PROPERTIES CXX_EXTENSIONS OFF)
target_compile_options(ray-tracer-lib PRIVATE ${COMPILE_FLAGS})
target_link_options(ray-tracer-lib PRIVATE ${LINK_FLAGS})
find_package(Threads REQUIRED)
target_link_libraries(ray-tracer-lib PRIVATE Threads::Threads)
target_link_libraries(ray-tracer-lib PUBLIC glm)

add_executable(run main.cpp)
target_link_libraries(run PRIVATE ray-tracer-lib)
target_compile_features(run PRIVATE cxx_std_17)
set_target_properties(run PROPERTIES CXX_EXTENSIONS OFF)
target_compile_options(run PRIVATE ${COMPILE_FLAGS})
target_link_options(run PRIVATE ${LINK_FLAGS})
