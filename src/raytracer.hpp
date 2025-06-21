#pragma once

#include <atomic>
#include <condition_variable>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include <glm/fwd.hpp>

#include "camera/camera.hpp"
#include "collisions/bvh_node.hpp"
#include "collisions/ray.hpp"
#include "lighting/light.hpp"
#include "materials/material.hpp"

namespace raytracer {

class Raytracer {
  private:
    const std::size_t WIDTH;
    const std::size_t HEIGHT;
    const std::size_t MAX_RECURSION_DEPTH;
    const std::size_t RAYS_PER_PIXEL;

    std::vector<glm::vec3> framebuffer;
    camera::Camera camera;
    std::map<std::string, std::unique_ptr<materials::Material>> materials;
    std::unique_ptr<collisions::BVHNode> bvh_root;
    std::vector<lighting::Light> lights;
    std::mutex mutex;
    std::condition_variable cv;
    std::atomic<std::size_t> finished_threads = 0;
    std::atomic<std::size_t> finished_lines = 0;
    std::atomic<std::size_t> rays_cast = 0;
    bool show_progress;

    void render_lines(std::size_t offset, std::size_t stride);
    glm::vec3 cast_ray(const collisions::Ray& ray, std::size_t recursion_depth);
    std::optional<collisions::Intersection> get_closest_intersection(const collisions::Ray& ray);
    glm::vec3 calculate_lighting(const collisions::Ray& ray, const collisions::Intersection& intersect, std::size_t recursion_depth);
    void create_simple_scene(std::size_t width, std::size_t height);
    void create_complex_scene(std::size_t width, std::size_t height);
    void create_two_spheres_scene(std::size_t width, std::size_t height);
    void create_earth_scene(std::size_t width, std::size_t height);
    void create_noise_scene(std::size_t width, std::size_t height);
    void create_quad_scene(std::size_t width, std::size_t height);
    void create_light_scene(std::size_t width, std::size_t height);

  public:
    Raytracer(std::size_t width, std::size_t height, std::size_t recursion_depth, std::size_t ray_per_pixel, bool show_progress);
    void trace_rays();
    void write_framebuffer(const std::string& filename) const;
    std::size_t get_rays_cast() const;
    std::size_t get_intersection_tests() const;
    void set_show_progress(bool show);
};

} // namespace raytracer
