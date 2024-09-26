#pragma once

#include <optional>

#include "aabb.hpp"
#include "intersection.hpp"
#include "ray.hpp"

namespace raytracer::collisions {

class Hittable {
  public:
    Hittable() = default;
    Hittable(const Hittable&) = default;
    Hittable(Hittable&&) = delete;
    Hittable& operator=(const Hittable&) = default;
    Hittable& operator=(Hittable&&) = delete;
    virtual ~Hittable() = default;
    virtual std::optional<Intersection> intersect(const Ray& ray) const = 0;
    virtual AABB bounding_box(float t0, float t1) const = 0;
};

} // namespace raytracer::collisions
