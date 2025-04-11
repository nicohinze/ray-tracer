#include <cstddef>
#include <optional>

#include "collisions/hittable.hpp"
#include "collisions/intersection.hpp"
#include "collisions/ray.hpp"

namespace raytracer::collisions {

std::optional<Intersection> Hittable::intersect(const Ray& ray) {
    ++intersection_tests;
    return intersect_impl(ray);
}

std::size_t Hittable::get_intersection_tests() const {
    return intersection_tests;
}

} // namespace raytracer::collisions
