#pragma once

#include <glm/ext/vector_float3.hpp>

namespace raytracer::collisions {

class Ray {
  private:
    glm::vec3 origin;
    glm::vec3 direction;
    float time;

  public:
    Ray(const glm::vec3& o, const glm::vec3& d, float t = 0);
    glm::vec3 get_origin() const;
    glm::vec3 get_direction() const;
    float get_time() const;
    glm::vec3 reflect(const glm::vec3& n) const;
    glm::vec3 refract(const glm::vec3& n, float refractive_index) const;
};

} // namespace raytracer::collisions
