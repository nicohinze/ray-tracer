#pragma once

#include <glm/ext/vector_float3.hpp>

#include "ray.hpp"

namespace raytracer::collisions {

class AABB {
  private:
    glm::vec3 min;
    glm::vec3 max;

    void pad_to_min();

  public:
    AABB();
    AABB(const glm::vec3& min, const glm::vec3& max);
    bool intersect(const Ray& ray) const;
    glm::vec3 get_min() const;
    glm::vec3 get_max() const;
};

AABB surrounding_box(const AABB& aabb0, const AABB& aabb1);

} // namespace raytracer::collisions
