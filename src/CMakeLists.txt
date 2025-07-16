cmake_minimum_required(VERSION 3.12)

set(TARGET ray-tracer)
set(SOURCE_FILES
    camera/camera.cpp
    collisions/aabb.cpp
    collisions/bvh_node.cpp
    collisions/hittable.cpp
    collisions/intersection.cpp
    collisions/ray.cpp
    collisions/rotation_y.cpp
    collisions/translation.cpp
    geometry/cuboid.cpp
    geometry/geometry_object.cpp
    geometry/moving_sphere.cpp
    geometry/quad.cpp
    geometry/sphere.cpp
    lighting/light.cpp
    materials/checker_texture.cpp
    materials/dielectric.cpp
    materials/diffuse_light.cpp
    materials/image_texture.cpp
    materials/lambertian.cpp
    materials/material.cpp
    materials/metal.cpp
    materials/noise_texture.cpp
    materials/solid_color.cpp
    noise/perlin.cpp
    raytracer.cpp
    utils/utils.cpp
)

add_library(${TARGET}-lib STATIC ${SOURCE_FILES})
target_compile_features(${TARGET}-lib PRIVATE cxx_std_20)
set_target_properties(${TARGET}-lib PROPERTIES CXX_EXTENSIONS OFF)
target_compile_options(${TARGET}-lib PRIVATE ${COMPILE_FLAGS})
target_link_options(${TARGET}-lib PRIVATE ${LINK_FLAGS})
find_package(Threads REQUIRED)
target_link_libraries(${TARGET}-lib PRIVATE Threads::Threads)
target_link_libraries(${TARGET}-lib PUBLIC glm)
target_include_directories(${TARGET}-lib PRIVATE "${PROJECT_SOURCE_DIR}/external")
target_include_directories(${TARGET}-lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${TARGET} main.cpp)
target_link_libraries(${TARGET} PRIVATE ${TARGET}-lib)
target_compile_features(${TARGET} PRIVATE cxx_std_20)
set_target_properties(${TARGET} PROPERTIES CXX_EXTENSIONS OFF)
target_compile_options(${TARGET} PRIVATE ${COMPILE_FLAGS})
target_link_options(${TARGET} PRIVATE ${LINK_FLAGS})
target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_command(
    TARGET ${TARGET}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data ${CMAKE_BINARY_DIR}/data
)
