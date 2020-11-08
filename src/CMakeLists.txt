cmake_minimum_required(VERSION 3.12)

set(
    SOURCE_FILES
    camera.cpp
    camera.hpp
    geometryobject.cpp
    geometryobject.hpp
    intersection.cpp
    intersection.hpp
    light.cpp
    light.hpp
    material.cpp
    material.hpp
    ray.cpp
    ray.hpp
    raytracer.cpp
    raytracer.hpp
    sphere.cpp
    sphere.hpp
    utils.cpp
    utils.hpp
)

add_library(ray-tracer-lib STATIC ${SOURCE_FILES})
target_compile_features(ray-tracer-lib PRIVATE cxx_std_17)
set_target_properties(ray-tracer-lib PROPERTIES CXX_EXTENSIONS OFF)
target_compile_options(ray-tracer-lib PRIVATE "-Wall;-Werror;-Wextra;")
find_package(Threads REQUIRED)
target_link_libraries(ray-tracer-lib PRIVATE Threads::Threads)
target_link_libraries(ray-tracer-lib PUBLIC glm)

add_executable(run main.cpp)
target_link_libraries(run PRIVATE ray-tracer-lib)
target_compile_features(run PRIVATE cxx_std_17)
set_target_properties(run PROPERTIES CXX_EXTENSIONS OFF)
target_compile_options(run PRIVATE "-Wall;-Werror;-Wextra;")
