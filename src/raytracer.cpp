#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <thread>

#include <glm/glm.hpp>

#include "geometryobject.hpp"
#include "light.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "raytracer.hpp"
#include "sphere.hpp"
#include "utils.hpp"

Raytracer::Raytracer(std::uint32_t width, std::uint32_t height, std::uint32_t recursion_depth, std::uint32_t ray_per_pixel)
    : WIDTH(width)
    , HEIGHT(height)
    , MAX_RECURSION_DEPTH(recursion_depth)
    , RAYS_PER_PIXEL(ray_per_pixel)
    , finished_threads(std::atomic<std::uint32_t>(0))
    , finished_lines(std::atomic<std::uint32_t>(0))
    , rays_cast(std::atomic<std::uint32_t>(0))
    , intersection_tests(std::atomic<std::uint32_t>(0))
    , show_progress(true) {
    framebuffer.reserve(width * height);
    create_simple_scene(width, height);
    // create_complex_scene(width, height);
}

void Raytracer::trace_rays() {
    const auto n_threads = std::thread::hardware_concurrency();
    const auto max_percent = 100;
    std::vector<std::thread> threads;
    for (auto i = 0U; i < n_threads; ++i) {
        auto t = std::thread(&Raytracer::render_lines, this, i, n_threads);
        threads.push_back(std::move(t));
    }
    {
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock, [this, &n_threads]() {
            if (show_progress) {
                show_render_progress(finished_lines * max_percent / HEIGHT);
            }
            return finished_threads == n_threads;
        });
    }
    for (auto& t : threads) {
        t.join();
    }
    if (show_progress) {
        show_render_progress(max_percent);
        std::cout << "\n";
    }
}

void Raytracer::write_framebuffer(const std::string& filename) {
    std::ofstream ofs;
    ofs.open(filename, std::ios_base::out | std::ios_base::trunc);
    ofs << "P6\n"
        << WIDTH << " " << HEIGHT << "\n255\n";
    for (std::uint32_t i = 0; i < WIDTH * HEIGHT; ++i) {
        for (std::uint32_t j = 0; j < 3; ++j) {
            ofs << static_cast<unsigned char>(255 * std::max(0.0F, std::min(1.0F, framebuffer[i][j])));
        }
    }
    ofs.close();
}

std::uint32_t Raytracer::get_rays_cast() const {
    return rays_cast;
}

std::uint32_t Raytracer::get_intersection_tests() const {
    return intersection_tests;
}

void Raytracer::set_show_progress(bool show) {
    show_progress = show;
}

void Raytracer::render_lines(std::uint32_t offset, std::uint32_t stride) {
    const auto seed = 19640;
    auto mt = std::mt19937(seed); // NOLINT(cert-msc32-c,cert-msc51-cpp)
    auto dist = std::uniform_real_distribution<float>(0.0, 1.0);
    for (std::uint32_t y = offset; y < HEIGHT; y += stride) {
        for (std::uint32_t x = 0; x < WIDTH; ++x) {
            auto avg_color = glm::vec3(0, 0, 0);
            for (std::uint32_t _ = 0; _ < RAYS_PER_PIXEL; ++_) {
                auto v = (static_cast<float>(HEIGHT) - static_cast<float>(y) + dist(mt)) / static_cast<float>(HEIGHT);
                auto u = (static_cast<float>(x) + dist(mt)) / static_cast<float>(WIDTH);
                auto ray = camera.get_ray(u, v);
                auto color = cast_ray(ray, 0);
                avg_color += color;
            }
            avg_color /= RAYS_PER_PIXEL;
            framebuffer[y * WIDTH + x] = avg_color;
        }
        finished_lines++;
        cv.notify_one();
    }
    finished_threads++;
    cv.notify_one();
}

glm::vec3 Raytracer::cast_ray(const Ray& ray, std::uint32_t recursion_depth) {
    static constexpr auto CYAN = glm::vec3(0.1, 1.0, 1.0);
    static constexpr auto BLUE = glm::vec3(0.1, 0.5, 1.0);
    if (recursion_depth > MAX_RECURSION_DEPTH) {
        return glm::vec3(0, 0, 0);
    }
    rays_cast++;
    auto closest_intersect = get_closest_intersection(ray);
    if (closest_intersect) {
        return calculate_lighting(ray, closest_intersect.value(), recursion_depth);
    }
    float t = 0.5F * (ray.get_direction().y + 1.0F);
    return (1.0F - t) * CYAN + t * BLUE;
}

std::optional<Intersection> Raytracer::get_closest_intersection(const Ray& ray) {
    std::optional<Intersection> closest_intersect;
    for (auto& object : geometry_objects) {
        intersection_tests++;
        if (auto intersect = object->intersect(ray)) {
            if (!closest_intersect || intersect->get_distance() < closest_intersect->get_distance()) {
                closest_intersect = intersect;
            }
        }
    }
    return closest_intersect;
}

glm::vec3 Raytracer::calculate_lighting(const Ray& ray, const Intersection& intersect, std::uint32_t recursion_depth) {
    if (intersect.get_material() != nullptr) {
        auto ambient = calculate_ambient_lighting(intersect);
        auto diffuse_and_specular = calculate_diffuse_and_specular_lighting(ray, intersect);
        auto reflec = calculate_reflective_lighting(ray, intersect, recursion_depth);
        auto refrac = calculate_refractive_lighting(ray, intersect, recursion_depth);
        return ambient + diffuse_and_specular + reflec + refrac;
    }
    return intersect.get_normal();
}

glm::vec3 Raytracer::calculate_ambient_lighting(const Intersection& intersect) {
    return intersect.get_material()->k_ambient * intersect.get_material()->color;
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
                                                                intersect.get_material()->shininess);
        }
    }
    return intersect.get_material()->k_diffuse * intersect.get_material()->color * diffuse_light_intensity +
           intersect.get_material()->k_specular * glm::vec3(1, 1, 1) * specular_light_intensity;
}

glm::vec3 Raytracer::calculate_reflective_lighting(const Ray& ray, const Intersection& intersect, std::uint32_t recursion_depth) {
    static const auto OFFSET = 1e-3F;
    if (intersect.get_material()->reflectivity > 0) {
        auto reflec_ray = Ray(intersect.get_position() + OFFSET * intersect.get_normal(), ray.reflect(intersect.get_normal()));
        return intersect.get_material()->reflectivity * cast_ray(reflec_ray, recursion_depth + 1);
    }
    return glm::vec3(0, 0, 0);
}

glm::vec3 Raytracer::calculate_refractive_lighting(const Ray& ray, const Intersection& intersect, std::uint32_t recursion_depth) {
    static const auto OFFSET = 1e-3F;
    if (intersect.get_material()->refractivity > 0) {
        auto refrac = ray.refract(intersect.get_normal(), intersect.get_material()->refractive_index);
        auto k = glm::dot(refrac, intersect.get_normal()) < 0 ? -1.0F : 1.0F;
        auto refrac_ray = Ray(intersect.get_position() + k * OFFSET * intersect.get_normal(), refrac);
        return intersect.get_material()->refractivity * cast_ray(refrac_ray, recursion_depth + 1);
    }
    return glm::vec3(0, 0, 0);
}

bool Raytracer::is_in_shadow(const Ray& ray, float light_distance) {
    if (auto intersect = get_closest_intersection(ray)) {
        return intersect->get_distance() < light_distance;
    }
    return false;
}

void Raytracer::create_simple_scene(std::uint32_t width, std::uint32_t height) {
    auto origin = glm::vec3(0, 0, 0);
    auto lookto = glm::vec3(0, 0, -1);
    auto vup = glm::vec3(0, 1, 0);
    auto vfov = 90.0F; // NOLINT(readability-magic-numbers)
    auto aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
    auto aperture = 0.75F;   // NOLINT(readability-magic-numbers)
    auto focus_dist = 16.0F; // NOLINT(readability-magic-numbers)
    camera = Camera(
        origin,
        origin + lookto,
        vup,
        vfov,
        aspect_ratio,
        aperture,
        focus_dist);

    materials["ivory"] = std::make_unique<Material>(glm::vec3(0.4, 0.4, 0.3), 0.1, 0.6, 0.3, 50.0, 0.1, 1.0, 0.0);      // NOLINT(readability-magic-numbers)
    materials["red_rubber"] = std::make_unique<Material>(glm::vec3(0.3, 0.1, 0.1), 0.1, 0.9, 0.1, 10.0, 0.0, 1.0, 0.0); // NOLINT(readability-magic-numbers)
    materials["mirror"] = std::make_unique<Material>(glm::vec3(1.0, 1.0, 1.0), 0.0, 0.0, 10.0, 1425.0, 0.8, 1.0, 0.0);  // NOLINT(readability-magic-numbers)
    materials["glass"] = std::make_unique<Material>(glm::vec3(0.6, 0.7, 0.8), 0.0, 0.0, 0.5, 125.0, 0.1, 1.5, 0.8);     // NOLINT(readability-magic-numbers)

    geometry_objects.push_back(std::make_unique<Sphere>(glm::vec3(-3, 0, -16), 2, materials["ivory"].get()));          // NOLINT(readability-magic-numbers)
    geometry_objects.push_back(std::make_unique<Sphere>(glm::vec3(-1.0, -1.5, -12), 2, materials["glass"].get()));     // NOLINT(readability-magic-numbers)
    geometry_objects.push_back(std::make_unique<Sphere>(glm::vec3(1.5, -0.5, -18), 3, materials["red_rubber"].get())); // NOLINT(readability-magic-numbers)
    geometry_objects.push_back(std::make_unique<Sphere>(glm::vec3(7, 5, -18), 4, materials["mirror"].get()));          // NOLINT(readability-magic-numbers)

    lights = {
        Light(glm::vec3(-20, 20, 20), 1.5), // NOLINT(readability-magic-numbers)
        Light(glm::vec3(30, 50, -25), 1.8), // NOLINT(readability-magic-numbers)
        Light(glm::vec3(30, 20, 30), 1.7)}; // NOLINT(readability-magic-numbers)
}

void Raytracer::create_complex_scene(std::uint32_t width, std::uint32_t height) {
    auto origin = glm::vec3(13, 2, 3); // NOLINT(readability-magic-numbers)
    auto lookat = glm::vec3(0, 0, 0);
    auto vup = glm::vec3(0, 1, 0);
    auto vfov = 20.0F; // NOLINT(readability-magic-numbers)
    auto aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
    auto aperture = 0.1F;    // NOLINT(readability-magic-numbers)
    auto focus_dist = 10.0F; // NOLINT(readability-magic-numbers)
    camera = Camera(
        origin,
        lookat,
        vup,
        vfov,
        aspect_ratio,
        aperture,
        focus_dist);

    materials["ground"] = std::make_unique<Material>(glm::vec3(0.5, 0.5, 0.5), 0.1, 0.4, 0.01, 1.0, 0.0, 1.0, 0.0);      // NOLINT(readability-magic-numbers)
    materials["big_diffuse"] = std::make_unique<Material>(glm::vec3(0.4, 0.2, 0.1), 0.1, 0.5, 0.0, 10.0, 0.0, 1.0, 0.0); // NOLINT(readability-magic-numbers)
    materials["big_metal"] = std::make_unique<Material>(glm::vec3(0.7, 0.6, 0.5), 0.0, 0.0, 1.0, 1000.0, 0.7, 1.0, 0.0); // NOLINT(readability-magic-numbers)
    materials["glass"] = std::make_unique<Material>(glm::vec3(0.6, 0.7, 0.8), 0.0, 0.0, 0.5, 125.0, 0.1, 1.5, 0.8);      // NOLINT(readability-magic-numbers)

    geometry_objects.push_back(std::make_unique<Sphere>(glm::vec3(0, -1000, 0), 1000, materials["ground"].get())); // NOLINT(readability-magic-numbers)
    geometry_objects.push_back(std::make_unique<Sphere>(glm::vec3(0, 1, 0), 1, materials["glass"].get()));         // NOLINT(readability-magic-numbers)
    geometry_objects.push_back(std::make_unique<Sphere>(glm::vec3(-4, 1, 0), 1, materials["big_diffuse"].get()));  // NOLINT(readability-magic-numbers)
    geometry_objects.push_back(std::make_unique<Sphere>(glm::vec3(4, 1, 0), 1, materials["big_metal"].get()));     // NOLINT(readability-magic-numbers)

    for (auto a = -11; a < 11; a++) {     // NOLINT(readability-magic-numbers)
        for (auto b = -11; b < 11; b++) { // NOLINT(readability-magic-numbers)
            auto choose_mat = random_float();
            auto center = glm::vec3(a + 0.9 * random_float(), 0.2, b + 0.9 * random_float()); // NOLINT(readability-magic-numbers)
            auto s = Sphere(center, 0.2, nullptr);                                            // NOLINT(readability-magic-numbers)
            bool intersect = false;
            for (const auto& object : geometry_objects) {
                if (s.intersect(*dynamic_cast<Sphere*>(object.get()))) {
                    intersect = true;
                    break;
                }
            }
            if (!intersect) {
                if (choose_mat < 0.8) { // NOLINT(readability-magic-numbers)
                    // diffuse
                    auto random_color = glm::vec3(random_float(), random_float(), random_float());
                    materials[std::to_string(a) + " " + std::to_string(b)] = std::make_unique<Material>(random_color, 0.1, 0.4, 0.0, 10.0, 0.0, 1.0, 0.0); // NOLINT(readability-magic-numbers)
                    geometry_objects.push_back(std::make_unique<Sphere>(center, 0.2, materials[std::to_string(a) + " " + std::to_string(b)].get()));       // NOLINT(readability-magic-numbers)
                } else if (choose_mat < 0.95) {                                                                                                            // NOLINT(readability-magic-numbers)
                    // metal
                    auto r = random_float();
                    materials[std::to_string(a) + " " + std::to_string(b)] = std::make_unique<Material>(glm::vec3(r, r, r), 0.0, 0.0, 1.0, 100.0, 0.5, 1.0, 0.0); // NOLINT(readability-magic-numbers)
                    geometry_objects.push_back(std::make_unique<Sphere>(center, 0.2, materials[std::to_string(a) + " " + std::to_string(b)].get()));              // NOLINT(readability-magic-numbers)
                } else {
                    // glass
                    geometry_objects.push_back(std::make_unique<Sphere>(center, 0.2, materials["glass"].get())); // NOLINT(readability-magic-numbers)
                }
            }
        }
    }
    lights = {
        Light(glm::vec3(-20, 20, 20), 1.5), // NOLINT(readability-magic-numbers)
        Light(glm::vec3(30, 50, -25), 1.8), // NOLINT(readability-magic-numbers)
        Light(glm::vec3(30, 20, 30), 1.7)}; // NOLINT(readability-magic-numbers)
}
