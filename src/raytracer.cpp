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
    framebuffer.reserve(WIDTH * HEIGHT);

    materials["ivory"] = std::make_unique<Material>(glm::vec3(0.4, 0.4, 0.3), 0.0, 0.6, 0.3, 50.0);      // NOLINT(readability-magic-numbers)
    materials["red_rubber"] = std::make_unique<Material>(glm::vec3(0.3, 0.1, 0.1), 0.0, 0.9, 0.1, 10.0); // NOLINT(readability-magic-numbers)

    geometry_objects.push_back(std::make_unique<Sphere>(glm::vec3(-3, 0, -16), 2, materials["ivory"].get()));           // NOLINT(readability-magic-numbers)
    geometry_objects.push_back(std::make_unique<Sphere>(glm::vec3(-1.0, -1.5, -12), 2, materials["red_rubber"].get())); // NOLINT(readability-magic-numbers)
    geometry_objects.push_back(std::make_unique<Sphere>(glm::vec3(1.5, -0.5, -18), 3, materials["red_rubber"].get()));  // NOLINT(readability-magic-numbers)
    geometry_objects.push_back(std::make_unique<Sphere>(glm::vec3(7, 5, -18), 4, materials["ivory"].get()));            // NOLINT(readability-magic-numbers)

    lights = {
        Light(glm::vec3(-20, 20, 20), 1.5), // NOLINT(readability-magic-numbers)
        Light(glm::vec3(30, 50, -25), 1.8), // NOLINT(readability-magic-numbers)
        Light(glm::vec3(30, 20, 30), 1.7)}; // NOLINT(readability-magic-numbers)
}

glm::vec3 Raytracer::cast_ray(const Ray& ray) {
    static constexpr auto CYAN = glm::vec3(0.1, 1.0, 1.0);
    static constexpr auto BLUE = glm::vec3(0.1, 0.5, 1.0);
    auto closest_intersect = get_closest_intersection(ray);
    if (closest_intersect) {
        return calculate_lighting(ray, closest_intersect.value());
    }
    float t = 0.5F * (ray.get_direction().y + 1.0F);
    return (1.0F - t) * CYAN + t * BLUE;
}

void Raytracer::cast_rays() {
    const auto lower_left = glm::vec3(-static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), -1.0, -1.0);
    const auto horizontal = glm::vec3(2 * static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 0, 0);
    const auto vertical = glm::vec3(0, 2, 0);

    for (int y = 0; y < HEIGHT; ++y) {
        auto v = static_cast<float>(HEIGHT - y) / static_cast<float>(HEIGHT);
        for (int x = 0; x < WIDTH; ++x) {
            auto u = static_cast<float>(x) / static_cast<float>(WIDTH);
            auto ray = Ray(camera.get_origin(), glm::normalize(lower_left + u * horizontal + v * vertical));
            framebuffer[y * WIDTH + x] = cast_ray(ray);
        }
    }
}

void Raytracer::write_framebuffer(const std::string& filename) {
    std::ofstream ofs;
    ofs.open(filename, std::ios_base::out | std::ios_base::trunc);
    ofs << "P6\n"
        << HEIGHT << " " << HEIGHT << "\n255\n";
    for (int i = 0; i < WIDTH * HEIGHT; ++i) {
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
            if (!closest_intersect || intersect->get_distance() < closest_intersect->get_distance()) {
                closest_intersect = intersect;
            }
        }
    }
    return closest_intersect;
}

glm::vec3 Raytracer::calculate_lighting(const Ray& ray, const Intersection& intersect) {
    if (intersect.get_material() != nullptr) {
        // auto ambient = calculate_ambient_lighting(intersect);
        auto diffuse = calculate_diffuse_and_specular_lighting(ray, intersect);
        return diffuse;
    }
    return intersect.get_normal();
}

glm::vec3 Raytracer::calculate_ambient_lighting(const Intersection& intersect) {
    return intersect.get_material()->get_k_ambient() * intersect.get_material()->get_color();
}

glm::vec3 Raytracer::calculate_diffuse_and_specular_lighting(const Ray& ray, const Intersection& intersect) {
    static constexpr auto OFFSET = 1e-3F;
    float diffuse_light_intensity = 0;
    float specular_light_intensity = 0;
    for (auto l : lights) {
        auto light_direction = glm::normalize(l.get_position() - intersect.get_position());
        auto shadow_ray = Ray(intersect.get_position() + OFFSET * intersect.get_normal(), light_direction);
        if (!is_in_shadow(shadow_ray, glm::length(l.get_position() - intersect.get_position()))) {
            diffuse_light_intensity += l.get_intensity() * std::max(0.0F, glm::dot(intersect.get_normal(), light_direction));
            specular_light_intensity += l.get_intensity() * std::pow(
                                                                std::max(0.0F, glm::dot(light_direction, ray.reflect(intersect.get_normal()))),
                                                                intersect.get_material()->get_shininess());
        }
    }
    return intersect.get_material()->get_k_diffuse() * intersect.get_material()->get_color() * diffuse_light_intensity +
           intersect.get_material()->get_k_specular() * glm::vec3(1, 1, 1) * specular_light_intensity;
}

bool Raytracer::is_in_shadow(const Ray& ray, float light_distance) {
    if (auto intersect = get_closest_intersection(ray)) {
        return intersect->get_distance() < light_distance;
    }
    return false;
}
