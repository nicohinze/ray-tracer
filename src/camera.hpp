#pragma once

#include <glm/glm.hpp>

#include "ray.hpp"

class Camera {
  private:
    glm::vec3 origin;
    glm::vec3 lower_left;
    glm::vec3 horizontal;
    glm::vec3 vertical;
    glm::vec3 u;
    glm::vec3 v;
    glm::vec3 w;
    float lens_radius;

  public:
    Camera();
    Camera(const glm::vec3& o, const glm::vec3& lookat, const glm::vec3& vup, float vfov, float aspect_ratio, float aperture, float focus_dist);
    Ray get_ray(float x, float y) const;
};
