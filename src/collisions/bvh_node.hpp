#pragma once

#include <atomic>
#include <memory>
#include <vector>

#include "aabb.hpp"
#include "hittable.hpp"
#include "intersection.hpp"

namespace raytracer::collisions {

class BVHNode : public Hittable {
  private:
    std::shared_ptr<Hittable> left;
    std::shared_ptr<Hittable> right;
    AABB aabb;
    static std::atomic<std::size_t> intersection_tests; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

  public:
    BVHNode(const std::vector<std::shared_ptr<Hittable>>& objects, float t0, float t1);
    BVHNode(const std::vector<std::shared_ptr<Hittable>>& objects, std::size_t start, std::size_t end, float t0, float t1);
    std::optional<Intersection> intersect(const Ray& ray) const override;
    AABB bounding_box(float t0, float t1) const override;
    static std::size_t get_intersection_tests();
};

} // namespace raytracer::collisions
