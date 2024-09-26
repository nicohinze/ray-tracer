#include <cmath>
#include <numbers>
#include <optional>
#include <utility>

#include <glm/exponential.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>

#include "collisions/aabb.hpp"
#include "collisions/intersection.hpp"
#include "collisions/ray.hpp"
#include "geometry/geometry_object.hpp"
#include "geometry/moving_sphere.hpp"
#include "materials/material.hpp"

namespace raytracer::geometry {

MovingSphere::MovingSphere(const glm::vec3 c1, const glm::vec3 c2, float r, float t1, float t2, const materials::Material* m)
    : GeometryObject(m), center1(c1), center2(c2), radius(r), time1(t1), time2(t2) {}

glm::vec3 MovingSphere::get_center(float time) const {
    return center1 + ((time - time1) / (time2 - time1)) * (center2 - center1);
}

std::optional<collisions::Intersection> MovingSphere::intersect(const collisions::Ray& ray) const {
    const auto origin = ray.get_origin();
    const auto direction = ray.get_direction();
    const auto center = get_center(ray.get_time());
    const auto r = center - origin;
    const auto s1 = glm::dot(direction, r);
    const auto d_2 = glm::length(r) * glm::length(r) - s1 * s1;
    if (radius * radius >= d_2) {
        const auto u2 = glm::sqrt(radius * radius - d_2);
        const auto t0 = s1 - u2;
        const auto t1 = s1 + u2;
        if (t0 >= 0) {
            const auto hit_point = origin + direction * t0;
            const auto outward_normal = glm::normalize(hit_point - center);
            const auto [u, v] = get_uv(outward_normal);
            return collisions::Intersection(t0, hit_point, outward_normal, material, u, v);
        }
        if (t1 >= 0) {
            const auto hit_point = origin + direction * t1;
            const auto outward_normal = glm::normalize(hit_point - center);
            const auto [u, v] = get_uv(outward_normal);
            return collisions::Intersection(t1, hit_point, outward_normal, material, u, v);
        }
    }
    return std::nullopt;
}

collisions::AABB MovingSphere::bounding_box(float t0, float t1) const {
    const auto aabb0 = collisions::AABB(get_center(t0) - glm::vec3(radius, radius, radius), get_center(t0) + glm::vec3(radius, radius, radius));
    const auto aabb1 = collisions::AABB(get_center(t1) - glm::vec3(radius, radius, radius), get_center(t1) + glm::vec3(radius, radius, radius));
    return surrounding_box(aabb0, aabb1);
}

std::pair<float, float> MovingSphere::get_uv(const glm::vec3& p) const {
    //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
    //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
    //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>
    const auto phi = std::atan2(-p.z, p.x) + static_cast<float>(std::numbers::pi);
    const auto theta = std::acos(-p.y);
    return std::make_pair(phi / (2 * std::numbers::pi), theta / std::numbers::pi);
}

} // namespace raytracer::geometry
