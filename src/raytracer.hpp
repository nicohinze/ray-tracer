#pragma once

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <map>
#include <memory>
#include <mutex>
#include <random>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "camera.hpp"
#include "light.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "sphere.hpp"

class Raytracer {
  private:
    const std::uint32_t WIDTH;
    const std::uint32_t HEIGHT;
    const std::uint32_t MAX_RECURSION_DEPTH;
    const std::uint32_t RAYS_PER_PIXEL;

    std::vector<glm::vec3> framebuffer;
    Camera camera;
    std::map<std::string, std::unique_ptr<Material>> materials;
    std::vector<std::unique_ptr<GeometryObject>> geometry_objects;
    std::vector<Light> lights;
    std::mutex mutex;
    std::condition_variable cv;
    std::atomic<std::uint32_t> finished_threads;
    std::atomic<std::uint32_t> finished_lines;
    std::atomic<std::uint32_t> rays_cast;
    std::atomic<std::uint32_t> intersection_tests;

    bool show_progress;

    void render_lines(std::uint32_t offset, std::uint32_t stride);
    glm::vec3 cast_ray(const Ray& ray, std::uint32_t recursion_depth);
    std::optional<Intersection> get_closest_intersection(const Ray& ray);
    glm::vec3 calculate_lighting(const Ray& ray, const Intersection& intersect, std::uint32_t recursion_depth);
    static glm::vec3 calculate_ambient_lighting(const Intersection& intersect);
    glm::vec3 calculate_diffuse_and_specular_lighting(const Ray& ray, const Intersection& intersect);
    glm::vec3 calculate_reflective_lighting(const Ray& ray, const Intersection& intersect, std::uint32_t recursion_depth);
    glm::vec3 calculate_refractive_lighting(const Ray& ray, const Intersection& intersect, std::uint32_t recursion_depth);
    bool is_in_shadow(const Ray& ray, float light_distance);
    void create_simple_scene(std::uint32_t width, std::uint32_t height);
    void create_complex_scene(std::uint32_t width, std::uint32_t height);

  public:
    Raytracer(std::uint32_t width, std::uint32_t height, std::uint32_t recursion_depth, std::uint32_t ray_per_pixel);
    void trace_rays();
    void write_framebuffer(const std::string& filename) const;
    std::uint32_t get_rays_cast() const;
    std::uint32_t get_intersection_tests() const;
    void set_show_progress(bool show);
};
