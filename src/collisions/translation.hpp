#pragma once

#include <memory>
#include <optional>

#include <glm/ext/vector_float3.hpp>

#include "collisions/aabb.hpp"
#include "collisions/hittable.hpp"
#include "collisions/intersection.hpp"
#include "collisions/ray.hpp"

namespace raytracer::collisions {

class Translation : public Hittable {
  private:
    std::shared_ptr<Hittable> object;
    glm::vec3 offset;
    AABB aabb;

  public:
    Translation(const std::shared_ptr<Hittable>& object, const glm::vec3& offset);
    std::optional<Intersection> intersect_impl(const Ray& ray) const override;
    AABB bounding_box(float t0, float t1) const override;
};

} // namespace raytracer::collisions
