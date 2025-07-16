#include <algorithm>
#include <cmath>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <random>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include <glm/ext/vector_float3.hpp>

#include "collisions/bvh_node.hpp"
#include "collisions/constant_medium.hpp"
#include "collisions/hittable.hpp"
#include "collisions/intersection.hpp"
#include "collisions/ray.hpp"
#include "collisions/rotation_y.hpp"
#include "collisions/translation.hpp"
#include "geometry/cuboid.hpp"
#include "geometry/moving_sphere.hpp"
#include "geometry/quad.hpp"
#include "geometry/sphere.hpp"
#include "materials/checker_texture.hpp"
#include "materials/dielectric.hpp"
#include "materials/diffuse_light.hpp"
#include "materials/image_texture.hpp"
#include "materials/lambertian.hpp"
#include "materials/metal.hpp"
#include "materials/noise_texture.hpp"
#include "raytracer.hpp"
#include "utils/utils.hpp"

namespace raytracer {

Raytracer::Raytracer(std::size_t width, std::size_t height, std::size_t recursion_depth, std::size_t ray_per_pixel, bool show_progress)
    : WIDTH(width)
    , HEIGHT(height)
    , MAX_RECURSION_DEPTH(recursion_depth)
    , RAYS_PER_PIXEL(ray_per_pixel)
    , show_progress(show_progress) {
    framebuffer.resize(width * height);
    // create_simple_scene(width, height);
    // create_complex_scene(width, height);
    // create_two_spheres_scene(width, height);
    // create_earth_scene(width, height);
    // create_noise_scene(width, height);
    // create_quad_scene(width, height);
    // create_light_scene(width, height);
    // create_cornell_box_scene(width, height);
    create_cornell_smoke_scene(width, height);
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
                utils::show_render_progress(finished_lines * max_percent / HEIGHT);
            }
            return finished_threads == n_threads;
        });
    }
    for (auto& t : threads) {
        t.join();
    }
    if (show_progress) {
        utils::show_render_progress(max_percent);
        std::cout << "\n";
    }
}

void Raytracer::write_framebuffer(const std::string& filename) const {
    std::ofstream ofs;
    ofs.open(filename, std::ios_base::out | std::ios_base::trunc);
    ofs << "P6\n"
        << WIDTH << ' ' << HEIGHT << "\n255\n";
    for (auto i = 0u; i < WIDTH * HEIGHT; ++i) {
        for (auto j = 0; j < 3; ++j) {
            ofs << static_cast<unsigned char>(255 * std::max(0.0F, std::min(1.0F, std::sqrt(framebuffer[i][j]))));
        }
    }
    ofs.close();
}

std::size_t Raytracer::get_rays_cast() const {
    return rays_cast;
}

std::size_t Raytracer::get_intersection_tests() const {
    return bvh_root->get_intersection_tests();
}

void Raytracer::set_show_progress(bool show) {
    show_progress = show;
}

void Raytracer::render_lines(std::size_t offset, std::size_t stride) {
    const auto seed = 19640;
    auto mt = std::mt19937(seed); // NOLINT(cert-msc32-c,cert-msc51-cpp)
    auto dist = std::uniform_real_distribution<float>(0.0, 1.0);
    for (auto y = offset; y < HEIGHT; y += stride) {
        for (auto x = 0u; x < WIDTH; ++x) {
            auto avg_color = glm::vec3(0, 0, 0);
            for (auto _ = 0u; _ < RAYS_PER_PIXEL; ++_) {
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

glm::vec3 Raytracer::cast_ray(const collisions::Ray& ray, std::size_t recursion_depth) {
    if (recursion_depth > MAX_RECURSION_DEPTH) {
        return {0, 0, 0};
    }
    rays_cast++;
    const auto closest_intersect = get_closest_intersection(ray);
    if (closest_intersect) {
        return calculate_lighting(ray, closest_intersect.value(), recursion_depth);
    }
    return camera.get_bg();
}

std::optional<collisions::Intersection> Raytracer::get_closest_intersection(const collisions::Ray& ray) {
    return bvh_root->intersect(ray);
}

glm::vec3 Raytracer::calculate_lighting(const collisions::Ray& ray, const collisions::Intersection& intersect, std::size_t recursion_depth) {
    if (intersect.get_material() != nullptr) {
        const auto [color, scattered] = intersect.get_material()->scatter(ray, intersect.get_position(), intersect.get_normal(), intersect.get_u(), intersect.get_v());
        const auto scatter_color = color * cast_ray(scattered, recursion_depth + 1);
        const auto emission_color = intersect.get_material()->emit(intersect.get_u(), intersect.get_v(), intersect.get_position());
        return scatter_color + emission_color;
    }
    return intersect.get_normal();
}

void Raytracer::create_simple_scene(std::size_t width, std::size_t height) {
    const auto origin = glm::vec3(0, 0, 0);
    const auto lookto = glm::vec3(0, 0, -1);
    const auto vup = glm::vec3(0, 1, 0);
    const auto bg = glm::vec3(0.7, 0.8, 1);
    const auto vfov = 90.0F;
    const auto aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
    const auto aperture = 0.75F;
    const auto focus_dist = 16.0F;
    camera = camera::Camera(
        origin,
        origin + lookto,
        vup,
        bg,
        vfov,
        aspect_ratio,
        aperture,
        focus_dist
    );

    materials["ivory"] = std::make_unique<materials::Lambertian>(glm::vec3(0.4, 0.4, 0.3));
    materials["red_rubber"] = std::make_unique<materials::Lambertian>(glm::vec3(0.9, 0.05, 0.05));
    materials["mirror"] = std::make_unique<materials::Metal>(glm::vec3(1.0, 1.0, 1.0), 0.0);
    materials["glass"] = std::make_unique<materials::Dielectric>(1.5);

    auto geometry_objects = std::vector<std::shared_ptr<collisions::Hittable>>();
    geometry_objects.push_back(std::make_shared<geometry::Sphere>(glm::vec3(-3, 0, -16), 2, materials["ivory"].get()));
    geometry_objects.push_back(std::make_shared<geometry::Sphere>(glm::vec3(-1.0, -1.5, -12), 2, materials["glass"].get()));
    geometry_objects.push_back(std::make_shared<geometry::Sphere>(glm::vec3(1.5, -0.5, -18), 3, materials["red_rubber"].get()));
    geometry_objects.push_back(std::make_shared<geometry::Sphere>(glm::vec3(7, 5, -18), 4, materials["mirror"].get()));
    bvh_root = std::make_unique<collisions::BVHNode>(geometry_objects, 0.0, 1.0);
}

void Raytracer::create_complex_scene(std::size_t width, std::size_t height) {
    const auto origin = glm::vec3(13, 2, 3);
    const auto lookat = glm::vec3(0, 0, 0);
    const auto vup = glm::vec3(0, 1, 0);
    const auto bg = glm::vec3(0.7, 0.8, 1);
    const auto vfov = 20.0F;
    const auto aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
    const auto aperture = 0.1F;
    const auto focus_dist = 10.0F;
    camera = camera::Camera(
        origin,
        lookat,
        vup,
        bg,
        vfov,
        aspect_ratio,
        aperture,
        focus_dist,
        0.0,
        1.0
    );

    materials["ground"] = std::make_unique<materials::Lambertian>(glm::vec3(0.5, 0.5, 0.5));
    materials["big_diffuse"] = std::make_unique<materials::Lambertian>(glm::vec3(0.4, 0.2, 0.1));
    materials["big_metal"] = std::make_unique<materials::Metal>(glm::vec3(0.7, 0.6, 0.5), 0.0);
    materials["glass"] = std::make_unique<materials::Dielectric>(1.5);

    auto geometry_objects = std::vector<std::shared_ptr<collisions::Hittable>>();
    geometry_objects.push_back(std::make_shared<geometry::Sphere>(glm::vec3(0, -1000, 0), 1000, materials["ground"].get()));
    geometry_objects.push_back(std::make_shared<geometry::Sphere>(glm::vec3(0, 1, 0), 1, materials["glass"].get()));
    geometry_objects.push_back(std::make_shared<geometry::Sphere>(glm::vec3(-4, 1, 0), 1, materials["big_diffuse"].get()));
    geometry_objects.push_back(std::make_shared<geometry::Sphere>(glm::vec3(4, 1, 0), 1, materials["big_metal"].get()));

    for (auto a = -11; a < 11; ++a) {
        for (auto b = -11; b < 11; ++b) {
            const auto choose_mat = utils::random_float();
            const auto center = glm::vec3(a + 0.9 * utils::random_float(), 0.2, b + 0.9 * utils::random_float());
            const auto s = geometry::Sphere(center, 0.2, nullptr);
            if (choose_mat < 0.8) {
                // diffuse
                const auto random_color = glm::vec3(utils::random_float(), utils::random_float(), utils::random_float()) * glm::vec3(utils::random_float(), utils::random_float(), utils::random_float());
                const auto center2 = center + glm::vec3(0, utils::random_float(0.0, 0.5), 0);
                materials[std::to_string(a) + " " + std::to_string(b)] = std::make_unique<materials::Lambertian>(random_color);
                geometry_objects.push_back(std::make_shared<geometry::MovingSphere>(center, center2, 0.2, 0.0, 1.0, materials[std::to_string(a) + " " + std::to_string(b)].get()));
            } else if (choose_mat < 0.95) {
                // metal
                const auto r = utils::random_float();
                const auto fuzz = utils::random_float(0.0, 0.5);
                materials[std::to_string(a) + " " + std::to_string(b)] = std::make_unique<materials::Metal>(glm::vec3(r, r, r), fuzz);
                geometry_objects.push_back(std::make_shared<geometry::Sphere>(center, 0.2, materials[std::to_string(a) + " " + std::to_string(b)].get()));
            } else {
                // glass
                geometry_objects.push_back(std::make_shared<geometry::Sphere>(center, 0.2, materials["glass"].get()));
            }
        }
    }
    bvh_root = std::make_unique<collisions::BVHNode>(geometry_objects, 0.0, 1.0);
}

void Raytracer::create_two_spheres_scene(std::size_t width, std::size_t height) {
    const auto origin = glm::vec3(13, 2, 3);
    const auto lookat = glm::vec3(0, 0, 0);
    const auto vup = glm::vec3(0, 1, 0);
    const auto bg = glm::vec3(0.7, 0.8, 1);
    const auto vfov = 20.0F;
    const auto aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
    const auto aperture = 0.1F;
    const auto focus_dist = 10.0F;
    camera = camera::Camera(
        origin,
        lookat,
        vup,
        bg,
        vfov,
        aspect_ratio,
        aperture,
        focus_dist,
        0.0,
        1.0
    );

    auto checker = std::make_unique<materials::CheckerTexture>(glm::vec3(0.2, 0.3, 0.1), glm::vec3(0.9, 0.9, 0.9));
    materials["checker"] = std::make_unique<materials::Lambertian>(std::move(checker));
    auto geometry_objects = std::vector<std::shared_ptr<collisions::Hittable>>();
    geometry_objects.push_back(std::make_shared<geometry::Sphere>(glm::vec3(0, -10, 0), 10, materials["checker"].get()));
    geometry_objects.push_back(std::make_shared<geometry::Sphere>(glm::vec3(0, 10, 0), 10, materials["checker"].get()));
    bvh_root = std::make_unique<collisions::BVHNode>(geometry_objects, 0.0, 1.0);
}

void Raytracer::create_earth_scene(std::size_t width, std::size_t height) {
    const auto origin = glm::vec3(13, 2, 3);
    const auto lookat = glm::vec3(0, 0, 0);
    const auto vup = glm::vec3(0, 1, 0);
    const auto bg = glm::vec3(0.7, 0.8, 1);
    const auto vfov = 20.0F;
    const auto aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
    const auto aperture = 0.1F;
    const auto focus_dist = 10.0F;
    camera = camera::Camera(
        origin,
        lookat,
        vup,
        bg,
        vfov,
        aspect_ratio,
        aperture,
        focus_dist,
        0.0,
        1.0
    );

    if (const auto data_dir = utils::data_directory_path()) {
        auto earth_texture = std::make_unique<materials::ImageTexture>((data_dir.value() / std::filesystem::path("earthmap.jpg")).string());
        materials["earth"] = std::make_unique<materials::Lambertian>(std::move(earth_texture));
    }
    auto geometry_objects = std::vector<std::shared_ptr<collisions::Hittable>>();
    geometry_objects.push_back(std::make_shared<geometry::Sphere>(glm::vec3(0, 0, 0), 2, materials["earth"].get()));
    bvh_root = std::make_unique<collisions::BVHNode>(geometry_objects, 0.0, 1.0);
}

void Raytracer::create_noise_scene(std::size_t width, std::size_t height) {
    const auto origin = glm::vec3(13, 2, 3);
    const auto lookat = glm::vec3(0, 0, 0);
    const auto vup = glm::vec3(0, 1, 0);
    const auto bg = glm::vec3(0.7, 0.8, 1);
    const auto vfov = 20.0F;
    const auto aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
    const auto aperture = 0.1F;
    const auto focus_dist = 10.0F;
    camera = camera::Camera(
        origin,
        lookat,
        vup,
        bg,
        vfov,
        aspect_ratio,
        aperture,
        focus_dist,
        0.0,
        1.0
    );

    auto noise_texture = std::make_unique<materials::NoiseTexture>(4);
    materials["noise"] = std::make_unique<materials::Lambertian>(std::move(noise_texture));
    auto geometry_objects = std::vector<std::shared_ptr<collisions::Hittable>>();
    geometry_objects.push_back(std::make_shared<geometry::Sphere>(glm::vec3(0, 0, 0), 2, materials["noise"].get()));
    bvh_root = std::make_unique<collisions::BVHNode>(geometry_objects, 0.0, 1.0);
}

void Raytracer::create_quad_scene(std::size_t width, std::size_t height) {
    const auto origin = glm::vec3(0, 0, 9);
    const auto lookat = glm::vec3(0, 0, 0);
    const auto vup = glm::vec3(0, 1, 0);
    const auto bg = glm::vec3(0.7, 0.8, 1);
    const auto vfov = 80.0f;
    const auto aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
    const auto aperture = 0.01f;
    const auto focus_dist = 10.0f;
    camera = camera::Camera(
        origin,
        lookat,
        vup,
        bg,
        vfov,
        aspect_ratio,
        aperture,
        focus_dist
    );

    materials["red"] = std::make_unique<materials::Lambertian>(glm::vec3(1.0, 0.2, 0.2));
    materials["green"] = std::make_unique<materials::Lambertian>(glm::vec3(0.2, 1.0, 0.2));
    materials["blue"] = std::make_unique<materials::Lambertian>(glm::vec3(0.2, 0.2, 1.0));
    materials["orange"] = std::make_unique<materials::Lambertian>(glm::vec3(1.0, 0.5, 0.0));
    materials["teal"] = std::make_unique<materials::Lambertian>(glm::vec3(0.2, 0.8, 0.8));
    auto geometry_objects = std::vector<std::shared_ptr<collisions::Hittable>>();
    geometry_objects.push_back(std::make_shared<geometry::Quad>(glm::vec3(-3, -2, 5), glm::vec3(0, 0, -4), glm::vec3(0, 4, 0), materials["red"].get()));
    geometry_objects.push_back(std::make_shared<geometry::Quad>(glm::vec3(-2, -2, 0), glm::vec3(4, 0, 0), glm::vec3(0, 4, 0), materials["green"].get()));
    geometry_objects.push_back(std::make_shared<geometry::Quad>(glm::vec3(3, -2, 1), glm::vec3(0, 0, 4), glm::vec3(0, 4, 0), materials["blue"].get()));
    geometry_objects.push_back(std::make_shared<geometry::Quad>(glm::vec3(-2, 3, 1), glm::vec3(4, 0, 0), glm::vec3(0, 0, 4), materials["orange"].get()));
    geometry_objects.push_back(std::make_shared<geometry::Quad>(glm::vec3(-2, -3, 5), glm::vec3(4, 0, 0), glm::vec3(0, 0, -4), materials["teal"].get()));
    bvh_root = std::make_unique<collisions::BVHNode>(geometry_objects, 0.0, 1.0);
}

void Raytracer::create_light_scene(std::size_t width, std::size_t height) {
    const auto origin = glm::vec3(26, 3, 6);
    const auto lookat = glm::vec3(0, 2, 0);
    const auto vup = glm::vec3(0, 1, 0);
    const auto bg = glm::vec3(0, 0, 0);
    const auto vfov = 20.0f;
    const auto aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
    const auto aperture = 0.01f;
    const auto focus_dist = 10.0f;
    camera = camera::Camera(
        origin,
        lookat,
        vup,
        bg,
        vfov,
        aspect_ratio,
        aperture,
        focus_dist
    );

    auto noise_texture = std::make_unique<materials::NoiseTexture>(4);
    materials["noise"] = std::make_unique<materials::Lambertian>(std::move(noise_texture));
    materials["light"] = std::make_unique<materials::DiffuseLight>(glm::vec3(4, 4, 4));
    auto geometry_objects = std::vector<std::shared_ptr<collisions::Hittable>>();
    geometry_objects.push_back(std::make_shared<geometry::Sphere>(glm::vec3(0, -1000, 0), 1000, materials["noise"].get()));
    geometry_objects.push_back(std::make_shared<geometry::Sphere>(glm::vec3(0, 2, 0), 2, materials["noise"].get()));
    geometry_objects.push_back(std::make_shared<geometry::Quad>(glm::vec3(3, 1, -2), glm::vec3(2, 0, 0), glm::vec3(0, 2, 0), materials["light"].get()));
    geometry_objects.push_back(std::make_shared<geometry::Sphere>(glm::vec3(0, 7, 0), 2, materials["light"].get()));
    bvh_root = std::make_unique<collisions::BVHNode>(geometry_objects, 0.0, 1.0);
}

void Raytracer::create_cornell_box_scene(std::size_t width, std::size_t height) {
    const auto origin = glm::vec3(278, 278, -800);
    const auto lookat = glm::vec3(278, 278, 0);
    const auto vup = glm::vec3(0, 1, 0);
    const auto bg = glm::vec3(0, 0, 0);
    const auto vfov = 40.0f;
    const auto aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
    const auto aperture = 0.01f;
    const auto focus_dist = 10.0f;
    camera = camera::Camera(
        origin,
        lookat,
        vup,
        bg,
        vfov,
        aspect_ratio,
        aperture,
        focus_dist
    );

    materials["red"] = std::make_unique<materials::Lambertian>(glm::vec3(0.65, 0.05, 0.05));
    materials["white"] = std::make_unique<materials::Lambertian>(glm::vec3(0.73, 0.73, 0.73));
    materials["green"] = std::make_unique<materials::Lambertian>(glm::vec3(0.12, 0.45, 0.15));
    materials["light"] = std::make_unique<materials::DiffuseLight>(glm::vec3(15, 15, 15));
    auto geometry_objects = std::vector<std::shared_ptr<collisions::Hittable>>();
    geometry_objects.push_back(std::make_shared<geometry::Quad>(glm::vec3(555, 0, 0), glm::vec3(0, 555, 0), glm::vec3(0, 0, 555), materials["green"].get()));
    geometry_objects.push_back(std::make_shared<geometry::Quad>(glm::vec3(0, 0, 0), glm::vec3(0, 555, 0), glm::vec3(0, 0, 555), materials["red"].get()));
    geometry_objects.push_back(std::make_shared<geometry::Quad>(glm::vec3(343, 554, 332), glm::vec3(-130, 0, 0), glm::vec3(0, 0, -105), materials["light"].get()));
    geometry_objects.push_back(std::make_shared<geometry::Quad>(glm::vec3(0, 0, 0), glm::vec3(555, 0, 0), glm::vec3(0, 0, 555), materials["white"].get()));
    geometry_objects.push_back(std::make_shared<geometry::Quad>(glm::vec3(555, 555, 555), glm::vec3(-555, 0, 0), glm::vec3(0, 0, -555), materials["white"].get()));
    geometry_objects.push_back(std::make_shared<geometry::Quad>(glm::vec3(0, 0, 555), glm::vec3(555, 0, 0), glm::vec3(0, 555, 0), materials["white"].get()));
    std::shared_ptr<collisions::Hittable> box1 = std::make_shared<geometry::Cuboid>(glm::vec3(0, 0, 0), glm::vec3(165, 330, 165), materials["white"].get());
    std::shared_ptr<collisions::Hittable> box2 = std::make_shared<geometry::Cuboid>(glm::vec3(0, 0, 0), glm::vec3(165, 165, 165), materials["white"].get());
    box1 = std::make_shared<collisions::RotationY>(box1, 15);
    box1 = std::make_shared<collisions::Translation>(box1, glm::vec3(265, 0, 295));
    box2 = std::make_shared<collisions::RotationY>(box2, -18);
    box2 = std::make_shared<collisions::Translation>(box2, glm::vec3(130, 0, 65));
    geometry_objects.push_back(box1);
    geometry_objects.push_back(box2);
    bvh_root = std::make_unique<collisions::BVHNode>(geometry_objects, 0.0, 1.0);
}

void Raytracer::create_cornell_smoke_scene(std::size_t width, std::size_t height) {
    const auto origin = glm::vec3(278, 278, -800);
    const auto lookat = glm::vec3(278, 278, 0);
    const auto vup = glm::vec3(0, 1, 0);
    const auto bg = glm::vec3(0, 0, 0);
    const auto vfov = 40.0f;
    const auto aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
    const auto aperture = 0.01f;
    const auto focus_dist = 10.0f;
    camera = camera::Camera(
        origin,
        lookat,
        vup,
        bg,
        vfov,
        aspect_ratio,
        aperture,
        focus_dist
    );

    materials["red"] = std::make_unique<materials::Lambertian>(glm::vec3(0.65, 0.05, 0.05));
    materials["white"] = std::make_unique<materials::Lambertian>(glm::vec3(0.73, 0.73, 0.73));
    materials["green"] = std::make_unique<materials::Lambertian>(glm::vec3(0.12, 0.45, 0.15));
    materials["light"] = std::make_unique<materials::DiffuseLight>(glm::vec3(15, 15, 15));
    auto geometry_objects = std::vector<std::shared_ptr<collisions::Hittable>>();
    geometry_objects.push_back(std::make_shared<geometry::Quad>(glm::vec3(555, 0, 0), glm::vec3(0, 555, 0), glm::vec3(0, 0, 555), materials["green"].get()));
    geometry_objects.push_back(std::make_shared<geometry::Quad>(glm::vec3(0, 0, 0), glm::vec3(0, 555, 0), glm::vec3(0, 0, 555), materials["red"].get()));
    geometry_objects.push_back(std::make_shared<geometry::Quad>(glm::vec3(343, 554, 332), glm::vec3(-130, 0, 0), glm::vec3(0, 0, -105), materials["light"].get()));
    geometry_objects.push_back(std::make_shared<geometry::Quad>(glm::vec3(0, 0, 0), glm::vec3(555, 0, 0), glm::vec3(0, 0, 555), materials["white"].get()));
    geometry_objects.push_back(std::make_shared<geometry::Quad>(glm::vec3(555, 555, 555), glm::vec3(-555, 0, 0), glm::vec3(0, 0, -555), materials["white"].get()));
    geometry_objects.push_back(std::make_shared<geometry::Quad>(glm::vec3(0, 0, 555), glm::vec3(555, 0, 0), glm::vec3(0, 555, 0), materials["white"].get()));
    std::shared_ptr<collisions::Hittable> box1 = std::make_shared<geometry::Cuboid>(glm::vec3(0, 0, 0), glm::vec3(165, 330, 165), materials["white"].get());
    std::shared_ptr<collisions::Hittable> box2 = std::make_shared<geometry::Cuboid>(glm::vec3(0, 0, 0), glm::vec3(165, 165, 165), materials["white"].get());
    box1 = std::make_shared<collisions::RotationY>(box1, 15);
    box1 = std::make_shared<collisions::Translation>(box1, glm::vec3(265, 0, 295));
    box2 = std::make_shared<collisions::RotationY>(box2, -18);
    box2 = std::make_shared<collisions::Translation>(box2, glm::vec3(130, 0, 65));
    geometry_objects.push_back(std::make_shared<collisions::ConstantMedium>(box1, 0.001f, glm::vec3(1, 1, 1)));
    geometry_objects.push_back(std::make_shared<collisions::ConstantMedium>(box2, 0.001f, glm::vec3(0, 0, 0)));
    bvh_root = std::make_unique<collisions::BVHNode>(geometry_objects, 0.0, 1.0);
}

} // namespace raytracer
