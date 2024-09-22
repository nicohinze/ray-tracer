#pragma once

#include <atomic>
#include <condition_variable>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "bvh_node.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "material.hpp"
#include "ray.hpp"

class Raytracer {
  private:
    const std::size_t WIDTH;
    const std::size_t HEIGHT;
    const std::size_t MAX_RECURSION_DEPTH;
    const std::size_t RAYS_PER_PIXEL;

    std::vector<glm::vec3> framebuffer;
    Camera camera;
    std::map<std::string, std::unique_ptr<Material>> materials;
    std::unique_ptr<BVHNode> bvh_root;
    std::vector<Light> lights;
    std::mutex mutex;
    std::condition_variable cv;
    std::atomic<std::size_t> finished_threads = 0;
    std::atomic<std::size_t> finished_lines = 0;
    std::atomic<std::size_t> rays_cast = 0;
    bool show_progress;

    void render_lines(std::size_t offset, std::size_t stride);
    glm::vec3 cast_ray(const Ray& ray, std::size_t recursion_depth);
    std::optional<Intersection> get_closest_intersection(const Ray& ray);
    glm::vec3 calculate_lighting(const Ray& ray, const Intersection& intersect, std::size_t recursion_depth);
    void create_simple_scene(std::size_t width, std::size_t height);
    void create_complex_scene(std::size_t width, std::size_t height);
    void create_two_spheres_scene(std::size_t width, std::size_t height);

  public:
    Raytracer(std::size_t width, std::size_t height, std::size_t recursion_depth, std::size_t ray_per_pixel);
    void trace_rays();
    void write_framebuffer(const std::string& filename) const;
    std::size_t get_rays_cast() const;
    std::size_t get_intersection_tests() const;
    void set_show_progress(bool show);
};
