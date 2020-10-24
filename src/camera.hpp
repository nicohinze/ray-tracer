#pragma once

#include <glm/glm.hpp>

#include "ray.hpp"

class Camera {
  private:
    glm::vec3 origin;
    glm::vec3 lower_left;
    glm::vec3 horizontal;
    glm::vec3 vertical;

  public:
    Camera(const glm::vec3& o, const glm::vec3& ll, const glm::vec3& h, const glm::vec3& v);
    Ray get_ray(float u, float v) const;
};
