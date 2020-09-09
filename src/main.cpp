#include <iostream>

#include "raytracer.hpp"
#include "sphere.hpp"

int main() {
    const auto width = 1024;
    const auto height = 768;
    const auto recursion_depth = 4;
    const auto rays_per_pixel = 100;
    auto raytracer = Raytracer(width, height, recursion_depth, rays_per_pixel);
    raytracer.trace_rays();
    raytracer.write_framebuffer("out.ppm");
}
