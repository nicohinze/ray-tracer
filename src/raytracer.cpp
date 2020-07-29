#include <fstream>
#include <iostream>
#include <memory>

#include <glm/glm.hpp>

#include "geometryobject.hpp"
#include "light.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "raytracer.hpp"
#include "sphere.hpp"

Raytracer::Raytracer() {
    framebuffer.reserve(width * height);

    materials["red"] = std::make_unique<Material>(glm::vec3(1, 0, 0));

    geometry_objects.push_back(std::make_unique<Sphere>(glm::vec3(5, 2, -7), 2, materials["red"].get()));
    geometry_objects.push_back(std::make_unique<Sphere>(glm::vec3(0, 0, -17), 2, nullptr));

    lights = {Light(glm::vec3(-20, 20, 20), 1.5)};
}

glm::vec3 Raytracer::cast_ray(const Ray& ray) {
    auto closest_intersect = Intersection(std::numeric_limits<float>::max(), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), nullptr);
    for (auto& object : geometry_objects) {
        if (auto intersect = object->intersect(ray)) {
            if (intersect->distance < closest_intersect.distance) {
                closest_intersect = intersect.value();
            }
        }
    }

    if (closest_intersect.distance < std::numeric_limits<float>::max()) {
        float light_intensity = 0;
        for (auto l : lights) {
            auto light_direction = glm::normalize(l.get_position() - closest_intersect.position);
            light_intensity += l.get_intensity() * std::max(0.0F, glm::dot(closest_intersect.normal, light_direction));
        }
        if (closest_intersect.material != nullptr) {
            return closest_intersect.material->get_diffuse_color() * light_intensity;
        }
        return closest_intersect.normal;
    }
    return glm::vec3(0.1, 0.5, 0.8);
}

void Raytracer::cast_rays() {
    const auto lower_left = glm::vec3(-static_cast<float>(width) / static_cast<float>(height), -1.0, -1.0);
    const auto horizontal = glm::vec3(2 * static_cast<float>(width) / static_cast<float>(height), 0, 0);
    const auto vertical = glm::vec3(0, 2, 0);

    for (int y = 0; y < height; ++y) {
        auto v = static_cast<float>(height - y) / static_cast<float>(height);
        for (int x = 0; x < width; ++x) {
            auto u = static_cast<float>(x) / static_cast<float>(width);
            auto ray = Ray(camera.get_origin(), glm::normalize(lower_left + u * horizontal + v * vertical));
            framebuffer[y * width + x] = cast_ray(ray);
        }
    }
}

void Raytracer::write_framebuffer(const std::string& filename) {
    std::ofstream ofs;
    ofs.open(filename);
    ofs << "P6\n"
        << width << " " << height << "\n255\n";
    for (int i = 0; i < height * width; ++i) {
        for (int j = 0; j < 3; ++j) {
            ofs << static_cast<char>(255 * std::max(0.0F, std::min(1.0F, framebuffer[i][j])));
        }
    }
    ofs.close();
}
