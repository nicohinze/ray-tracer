#pragma once

#include <atomic>
#include <optional>

#include "aabb.hpp"
#include "intersection.hpp"
#include "ray.hpp"

namespace raytracer::collisions {

class Hittable {
  protected:
    std::atomic<std::size_t> intersection_tests;

  public:
    Hittable() = default;
    Hittable(const Hittable&) = delete;
    Hittable(Hittable&&) = delete;
    Hittable& operator=(const Hittable&) = delete;
    Hittable& operator=(Hittable&&) = delete;
    virtual ~Hittable() = default;
    std::size_t get_intersection_tests() const;
    std::optional<Intersection> intersect(const Ray& ray);
    virtual std::optional<Intersection> intersect_impl(const Ray& ray) const = 0;
    virtual AABB bounding_box(float t0, float t1) const = 0;
};

} // namespace raytracer::collisions
