#pragma once

#include <glm/ext/vector_float3.hpp>

#include "collisions/ray.hpp"

namespace raytracer::camera {

class Camera {
  private:
    glm::vec3 origin;
    glm::vec3 lower_left;
    glm::vec3 horizontal;
    glm::vec3 vertical;
    glm::vec3 u;
    glm::vec3 v;
    glm::vec3 w;
    glm::vec3 bg;
    float lens_radius;
    float time1;
    float time2;

  public:
    Camera();
    Camera(const glm::vec3& o, const glm::vec3& lookat, const glm::vec3& vup, const glm::vec3& bg, float vfov, float aspect_ratio, float aperture, float focus_dist, float t1 = 0, float t2 = 0);
    collisions::Ray get_ray(float x, float y) const;
    glm::vec3 get_bg() const;
};

} // namespace raytracer::camera
