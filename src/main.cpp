#include <iostream>

#include "raytracer.hpp"
#include "sphere.hpp"

int main() {
    Raytracer raytracer;
    raytracer.trace_rays();
    raytracer.write_framebuffer("out.ppm");
}
