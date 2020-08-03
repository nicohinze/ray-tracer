#include <cmath>
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
    auto closest_intersect = get_closest_intersection(ray);
    if (closest_intersect) {
        return calculate_lighting(ray, closest_intersect.value());
    }
    float t = 0.5 * (ray.direction.y + 1.0);
    return (1.0F - t) * glm::vec3(0.1, 1.0, 1.0) + t * glm::vec3(0.1, 0.5, 1.0);
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

std::optional<Intersection> Raytracer::get_closest_intersection(const Ray& ray) {
    std::optional<Intersection> closest_intersect;
    for (auto& object : geometry_objects) {
        if (auto intersect = object->intersect(ray)) {
            if (!closest_intersect || intersect->distance < closest_intersect->distance) {
                closest_intersect = intersect;
            }
        }
    }
    return closest_intersect;
}

glm::vec3 Raytracer::calculate_lighting(const Ray& ray, const Intersection& intersect) {
    if (intersect.material != nullptr) {
        // auto ambient = calculate_ambient_lighting(intersect);
        auto diffuse = calculate_diffuse_and_specular_lighting(ray, intersect);
        return diffuse;
    }
    return intersect.normal;
}

glm::vec3 Raytracer::calculate_ambient_lighting(const Intersection& intersect) {
    return intersect.material->get_k_ambient() * intersect.material->get_color();
}

glm::vec3 Raytracer::calculate_diffuse_and_specular_lighting(const Ray& ray, const Intersection& intersect) {
    float diffuse_light_intensity = 0;
    float specular_light_intensity = 0;
    for (auto l : lights) {
        auto light_direction = glm::normalize(l.get_position() - intersect.position);
        diffuse_light_intensity += l.get_intensity() * std::max(0.0F, glm::dot(intersect.normal, light_direction));
        specular_light_intensity += l.get_intensity() * std::pow(
                                                            std::max(0.0F, glm::dot(light_direction, ray.reflect(intersect.normal))),
                                                            intersect.material->get_shininess());
    }
    return intersect.material->get_k_diffuse() * intersect.material->get_color() * diffuse_light_intensity +
           intersect.material->get_k_specular() * glm::vec3(1, 1, 1) * specular_light_intensity;
}
