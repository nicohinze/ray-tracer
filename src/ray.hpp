#pragma once

#include <glm/glm.hpp>

class Ray {
  private:
    glm::vec3 origin;
    glm::vec3 direction;

  public:
    Ray(const glm::vec3& o, const glm::vec3& d);
    glm::vec3 get_origin() const;
    glm::vec3 get_direction() const;
    glm::vec3 reflect(const glm::vec3& n) const;
};
