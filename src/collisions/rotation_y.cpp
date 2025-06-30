#include <cmath>
#include <limits>
#include <memory>
#include <optional>

#include <glm/ext/vector_float3.hpp>
#include <glm/trigonometric.hpp>

#include "collisions/aabb.hpp"
#include "collisions/hittable.hpp"
#include "collisions/intersection.hpp"
#include "collisions/ray.hpp"
#include "collisions/rotation_y.hpp"

namespace raytracer::collisions {
RotationY::RotationY(const std::shared_ptr<Hittable>& object, float theta)
    : object(object)
    , sin_theta(std::sin(glm::radians(theta)))
    , cos_theta(std::cos(glm::radians(theta)))
    , aabb(object->bounding_box(0, 0)) {
    const auto inf = std::numeric_limits<float>::infinity();
    auto min = glm::vec3(inf, inf, inf);
    auto max = glm::vec3(-inf, -inf, -inf);
    for (auto n = 0u; n < 8; ++n) {
        const auto x = ((n & 1u) != 0) ? aabb.get_max().x : aabb.get_min().x;
        const auto y = ((n & 2u) != 0) ? aabb.get_max().y : aabb.get_min().y;
        const auto z = ((n & 4u) != 0) ? aabb.get_max().z : aabb.get_min().z;
        const auto new_x = cos_theta * x + sin_theta * z;
        const auto new_z = -sin_theta * x + cos_theta * z;
        const auto p = glm::vec3(new_x, y, new_z);
        for (auto c = 0; c < 3; ++c) {
            min[c] = std::fmin(min[c], p[c]);
            max[c] = std::fmax(max[c], p[c]);
        }
    }
    aabb = AABB(min, max);
}

std::optional<Intersection> RotationY::intersect_impl(const Ray& ray) const {
    const auto origin = glm::vec3(
        cos_theta * ray.get_origin().x - sin_theta * ray.get_origin().z,
        ray.get_origin().y,
        sin_theta * ray.get_origin().x + cos_theta * ray.get_origin().z
    );
    const auto direction = glm::vec3(
        cos_theta * ray.get_direction().x - sin_theta * ray.get_direction().z,
        ray.get_direction().y,
        sin_theta * ray.get_direction().x + cos_theta * ray.get_direction().z
    );
    const auto rotated_ray = Ray(origin, direction);
    return object->intersect_impl(rotated_ray);
}

AABB RotationY::bounding_box(float /*t0*/, float /*t1*/) const {
    return aabb;
}

} // namespace raytracer::collisions
