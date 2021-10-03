#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <thread>

#include <glm/glm.hpp>

#include "bvh_node.hpp"
#include "checker_texture.hpp"
#include "dielectric.hpp"
#include "geometry_object.hpp"
#include "lambertian.hpp"
#include "light.hpp"
#include "metal.hpp"
#include "moving_sphere.hpp"
#include "ray.hpp"
#include "raytracer.hpp"
#include "sphere.hpp"
#include "utils.hpp"

Raytracer::Raytracer(std::uint32_t width, std::uint32_t height, std::uint32_t recursion_depth, std::uint32_t ray_per_pixel)
    : WIDTH(width)
    , HEIGHT(height)
    , MAX_RECURSION_DEPTH(recursion_depth)
    , RAYS_PER_PIXEL(ray_per_pixel)
    , show_progress(true) {
    framebuffer.reserve(width * height);
    // create_simple_scene(width, height);
    create_complex_scene(width, height);
    // create_two_spheres_scene(width, height);
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

void Raytracer::write_framebuffer(const std::string& filename) const {
    std::ofstream ofs;
    ofs.open(filename, std::ios_base::out | std::ios_base::trunc);
    ofs << "P6\n"
        << WIDTH << " " << HEIGHT << "\n255\n";
    for (std::uint32_t i = 0; i < WIDTH * HEIGHT; ++i) {
        for (std::uint32_t j = 0; j < 3; ++j) {
            ofs << static_cast<unsigned char>(255 * std::max(0.0F, std::min(1.0F, std::sqrt(framebuffer[i][j]))));
        }
    }
    ofs.close();
}

std::uint64_t Raytracer::get_rays_cast() const {
    return rays_cast;
}

std::uint64_t Raytracer::get_intersection_tests() const {
    return bvh_root->get_intersection_tests();
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
                const auto v = (static_cast<float>(HEIGHT) - static_cast<float>(y) + dist(mt)) / static_cast<float>(HEIGHT);
                const auto u = (static_cast<float>(x) + dist(mt)) / static_cast<float>(WIDTH);
                const auto ray = camera.get_ray(u, v);
                const auto color = cast_ray(ray, 0);
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
    static constexpr auto WHITE = glm::vec3(1.0, 1.0, 1.0);
    static constexpr auto LIGHT_BLUE = glm::vec3(0.5, 0.7, 1.0);
    if (recursion_depth > MAX_RECURSION_DEPTH) {
        return glm::vec3(0, 0, 0);
    }
    rays_cast++;
    const auto closest_intersect = get_closest_intersection(ray);
    if (closest_intersect) {
        return calculate_lighting(ray, closest_intersect.value(), recursion_depth);
    }
    const float t = 0.5F * (ray.get_direction().y + 1.0F);
    return (1.0F - t) * WHITE + t * LIGHT_BLUE;
}

std::optional<Intersection> Raytracer::get_closest_intersection(const Ray& ray) {
    return bvh_root->intersect(ray);
}

glm::vec3 Raytracer::calculate_lighting(const Ray& ray, const Intersection& intersect, std::uint32_t recursion_depth) {
    if (intersect.get_material() != nullptr) {
        const auto [color, scattered] = intersect.get_material()->scatter(ray, intersect.get_position(), intersect.get_normal(), intersect.get_u(), intersect.get_v());
        return color * cast_ray(scattered, recursion_depth + 1);
    }
    return intersect.get_normal();
}

void Raytracer::create_simple_scene(std::uint32_t width, std::uint32_t height) {
    const auto origin = glm::vec3(0, 0, 0);
    const auto lookto = glm::vec3(0, 0, -1);
    const auto vup = glm::vec3(0, 1, 0);
    const auto vfov = 90.0F; // NOLINT(readability-magic-numbers)
    const auto aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
    const auto aperture = 0.75F;   // NOLINT(readability-magic-numbers)
    const auto focus_dist = 16.0F; // NOLINT(readability-magic-numbers)
    camera = Camera(
        origin,
        origin + lookto,
        vup,
        vfov,
        aspect_ratio,
        aperture,
        focus_dist);

    materials["ivory"] = std::make_unique<Lambertian>(glm::vec3(0.4, 0.4, 0.3));        // NOLINT(readability-magic-numbers)
    materials["red_rubber"] = std::make_unique<Lambertian>(glm::vec3(0.9, 0.05, 0.05)); // NOLINT(readability-magic-numbers)
    materials["mirror"] = std::make_unique<Metal>(glm::vec3(1.0, 1.0, 1.0), 0.0);       // NOLINT(readability-magic-numbers)
    materials["glass"] = std::make_unique<Dielectric>(1.5);                             // NOLINT(readability-magic-numbers)

    auto geometry_objects = std::vector<std::shared_ptr<Hittable>>();
    geometry_objects.push_back(std::make_shared<Sphere>(glm::vec3(-3, 0, -16), 2, materials["ivory"].get()));          // NOLINT(readability-magic-numbers)
    geometry_objects.push_back(std::make_shared<Sphere>(glm::vec3(-1.0, -1.5, -12), 2, materials["glass"].get()));     // NOLINT(readability-magic-numbers)
    geometry_objects.push_back(std::make_shared<Sphere>(glm::vec3(1.5, -0.5, -18), 3, materials["red_rubber"].get())); // NOLINT(readability-magic-numbers)
    geometry_objects.push_back(std::make_shared<Sphere>(glm::vec3(7, 5, -18), 4, materials["mirror"].get()));          // NOLINT(readability-magic-numbers)
    bvh_root = std::make_unique<BVH_Node>(geometry_objects, 0.0, 1.0);
}

void Raytracer::create_complex_scene(std::uint32_t width, std::uint32_t height) {
    const auto origin = glm::vec3(13, 2, 3); // NOLINT(readability-magic-numbers)
    const auto lookat = glm::vec3(0, 0, 0);
    const auto vup = glm::vec3(0, 1, 0);
    const auto vfov = 20.0F; // NOLINT(readability-magic-numbers)
    const auto aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
    const auto aperture = 0.1F;    // NOLINT(readability-magic-numbers)
    const auto focus_dist = 10.0F; // NOLINT(readability-magic-numbers)
    camera = Camera(
        origin,
        lookat,
        vup,
        vfov,
        aspect_ratio,
        aperture,
        focus_dist,
        0.0,
        1.0);

    materials["ground"] = std::make_unique<Lambertian>(glm::vec3(0.5, 0.5, 0.5));      // NOLINT(readability-magic-numbers)
    materials["big_diffuse"] = std::make_unique<Lambertian>(glm::vec3(0.4, 0.2, 0.1)); // NOLINT(readability-magic-numbers)
    materials["big_metal"] = std::make_unique<Metal>(glm::vec3(0.7, 0.6, 0.5), 0.0);   // NOLINT(readability-magic-numbers)
    materials["glass"] = std::make_unique<Dielectric>(1.5);                            // NOLINT(readability-magic-numbers)

    auto geometry_objects = std::vector<std::shared_ptr<Hittable>>();
    geometry_objects.push_back(std::make_shared<Sphere>(glm::vec3(0, -1000, 0), 1000, materials["ground"].get())); // NOLINT(readability-magic-numbers)
    geometry_objects.push_back(std::make_shared<Sphere>(glm::vec3(0, 1, 0), 1, materials["glass"].get()));         // NOLINT(readability-magic-numbers)
    geometry_objects.push_back(std::make_shared<Sphere>(glm::vec3(-4, 1, 0), 1, materials["big_diffuse"].get()));  // NOLINT(readability-magic-numbers)
    geometry_objects.push_back(std::make_shared<Sphere>(glm::vec3(4, 1, 0), 1, materials["big_metal"].get()));     // NOLINT(readability-magic-numbers)

    for (auto a = -11; a < 11; ++a) {     // NOLINT(readability-magic-numbers)
        for (auto b = -11; b < 11; ++b) { // NOLINT(readability-magic-numbers)
            const auto choose_mat = random_float();
            const auto center = glm::vec3(a + 0.9 * random_float(), 0.2, b + 0.9 * random_float()); // NOLINT(readability-magic-numbers)
            const auto s = Sphere(center, 0.2, nullptr);                                            // NOLINT(readability-magic-numbers)
            if (choose_mat < 0.8) {                                                                 // NOLINT(readability-magic-numbers)
                // diffuse
                const auto random_color = glm::vec3(random_float(), random_float(), random_float()) * glm::vec3(random_float(), random_float(), random_float());
                const auto center2 = center + glm::vec3(0, random_float(0.0, 0.5), 0);
                materials[std::to_string(a) + " " + std::to_string(b)] = std::make_unique<Lambertian>(random_color);                                                      // NOLINT(readability-magic-numbers)
                geometry_objects.push_back(std::make_shared<MovingSphere>(center, center2, 0.2, 0.0, 1.0, materials[std::to_string(a) + " " + std::to_string(b)].get())); // NOLINT(readability-magic-numbers)
            } else if (choose_mat < 0.95) {                                                                                                                               // NOLINT(readability-magic-numbers)
                // metal
                const auto r = random_float();
                const auto fuzz = random_float(0.0, 0.5);
                materials[std::to_string(a) + " " + std::to_string(b)] = std::make_unique<Metal>(glm::vec3(r, r, r), fuzz);                      // NOLINT(readability-magic-numbers)
                geometry_objects.push_back(std::make_shared<Sphere>(center, 0.2, materials[std::to_string(a) + " " + std::to_string(b)].get())); // NOLINT(readability-magic-numbers)
            } else {
                // glass
                geometry_objects.push_back(std::make_shared<Sphere>(center, 0.2, materials["glass"].get())); // NOLINT(readability-magic-numbers)
            }
        }
    }
    bvh_root = std::make_unique<BVH_Node>(geometry_objects, 0.0, 1.0);
}

void Raytracer::create_two_spheres_scene(std::uint32_t width, std::uint32_t height) {
    const auto origin = glm::vec3(13, 2, 3); // NOLINT(readability-magic-numbers)
    const auto lookat = glm::vec3(0, 0, 0);
    const auto vup = glm::vec3(0, 1, 0);
    const auto vfov = 20.0F; // NOLINT(readability-magic-numbers)
    const auto aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
    const auto aperture = 0.1F;    // NOLINT(readability-magic-numbers)
    const auto focus_dist = 10.0F; // NOLINT(readability-magic-numbers)
    camera = Camera(
        origin,
        lookat,
        vup,
        vfov,
        aspect_ratio,
        aperture,
        focus_dist,
        0.0,
        1.0);

    auto checker = std::make_unique<CheckerTexture>(glm::vec3(0.2, 0.3, 0.1), glm::vec3(0.9, 0.9, 0.9)); // NOLINT(readability-magic-numbers)
    materials["checker"] = std::make_unique<Lambertian>(std::move(checker));
    auto geometry_objects = std::vector<std::shared_ptr<Hittable>>();
    geometry_objects.push_back(std::make_shared<Sphere>(glm::vec3(0, -10, 0), 10, materials["checker"].get())); // NOLINT(readability-magic-numbers)
    geometry_objects.push_back(std::make_shared<Sphere>(glm::vec3(0, 10, 0), 10, materials["checker"].get()));  // NOLINT(readability-magic-numbers)
    bvh_root = std::make_unique<BVH_Node>(geometry_objects, 0.0, 1.0);
}
