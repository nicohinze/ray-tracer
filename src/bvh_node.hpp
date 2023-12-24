#pragma once

#include <atomic>

#include "aabb.hpp"
#include "hittable.hpp"
#include "intersection.hpp"

class BVH_Node : public Hittable {
  private:
    std::shared_ptr<Hittable> left;
    std::shared_ptr<Hittable> right;
    AABB aabb;
    static std::atomic<std::size_t> intersection_tests; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

  public:
    BVH_Node(const std::vector<std::shared_ptr<Hittable>>& objects, float t0, float t1);
    BVH_Node(const std::vector<std::shared_ptr<Hittable>>& objects, std::size_t start, std::size_t end, float t0, float t1);
    std::optional<Intersection> intersect(const Ray& ray) const override;
    std::optional<AABB> bounding_box(float t0, float t1) const override;
    static std::size_t get_intersection_tests();
};
