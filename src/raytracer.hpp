#pragma once

#include <map>
#include <memory>
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
    const int WIDTH = 1024;
    const int HEIGHT = 768;
    std::vector<glm::vec3> framebuffer;
    Camera camera;
    std::map<std::string, std::unique_ptr<Material>> materials;
    std::vector<std::unique_ptr<GeometryObject>> geometry_objects;
    std::vector<Light> lights;

    glm::vec3 cast_ray(const Ray& ray);

  public:
    Raytracer();
    void cast_rays();
    void write_framebuffer(const std::string& filename);
    std::optional<Intersection> get_closest_intersection(const Ray& ray);
    glm::vec3 calculate_lighting(const Ray& ray, const Intersection& intersect);
    glm::vec3 calculate_ambient_lighting(const Intersection& intersect);
    glm::vec3 calculate_diffuse_and_specular_lighting(const Ray& ray, const Intersection& intersect);
    bool is_in_shadow(const Ray& ray, float light_distance);
};
