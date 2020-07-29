#include <iostream>

#include "raytracer.hpp"
#include "sphere.hpp"

int main() {
    Raytracer raytracer;
    raytracer.cast_rays();
    raytracer.write_framebuffer("out.ppm");
}
