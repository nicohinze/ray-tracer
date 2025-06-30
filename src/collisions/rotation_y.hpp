#pragma once

#include <memory>
#include <optional>

#include "collisions/aabb.hpp"
#include "collisions/hittable.hpp"
#include "collisions/intersection.hpp"
#include "collisions/ray.hpp"

namespace raytracer::collisions {

class RotationY : public Hittable {
  private:
    std::shared_ptr<Hittable> object;
    float sin_theta;
    float cos_theta;
    AABB aabb;

  public:
    RotationY(const std::shared_ptr<Hittable>& object, float theta);
    std::optional<Intersection> intersect_impl(const Ray& ray) const override;
    AABB bounding_box(float t0, float t1) const override;
};

} // namespace raytracer::collisions
