#pragma once

#include <optional>

#include "aabb.hpp"
#include "intersection.hpp"
#include "ray.hpp"

class Hittable {
  public:
    virtual ~Hittable() = default;
    virtual std::optional<Intersection> intersect(const Ray& ray) const = 0;
    virtual std::optional<AABB> bounding_box(float t0, float t1) const = 0;
};
