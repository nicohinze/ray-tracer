#include <chrono>
#include <iomanip>
#include <iostream>

#include "raytracer.hpp"

int main() {
    const auto enable_output = true;
    const auto width = 1920;
    const auto height = 1080;
    const auto recursion_depth = 100;
    const auto rays_per_pixel = 100;
    auto raytracer = raytracer::Raytracer(width, height, recursion_depth, rays_per_pixel, enable_output);
    auto start = std::chrono::steady_clock::now();
    raytracer.trace_rays();
    auto stop = std::chrono::steady_clock::now();
    auto elapsed = stop - start;
    auto min = std::chrono::duration_cast<std::chrono::minutes>(elapsed);
    auto s = std::chrono::duration_cast<std::chrono::seconds>(elapsed);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
    ms = std::chrono::duration_cast<std::chrono::milliseconds>(ms - s);
    s = std::chrono::duration_cast<std::chrono::seconds>(s - min);
    if (enable_output) {
        std::cout << "Rendering took: "
                  << std::setfill('0') << std::setw(2) << min.count() << ':'
                  << std::setfill('0') << std::setw(2) << s.count() << '.'
                  << std::setfill('0') << std::setw(3) << ms.count() << '\n'
                  << "Rays cast: " << raytracer.get_rays_cast() << '\n'
                  << "Intersection tests: " << raytracer.get_intersection_tests() << '\n';
    }
    raytracer.write_framebuffer("out.ppm");
}
