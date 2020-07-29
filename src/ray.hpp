#pragma once

#include <glm/glm.hpp>

class Ray {
  private:
  public:
    glm::vec3 origin;
    glm::vec3 direction;

    Ray(const glm::vec3& o, const glm::vec3& d);
    ~Ray() = default;
};
