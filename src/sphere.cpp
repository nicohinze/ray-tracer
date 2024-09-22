#include <cmath>
#include <numbers>
#include <optional>
#include <utility>

#include <glm/exponential.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>

#include "aabb.hpp"
#include "geometry_object.hpp"
#include "intersection.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "sphere.hpp"

Sphere::Sphere(const glm::vec3& c, float r, const Material* m)
    : GeometryObject(m), center(c), radius(r) {}

std::optional<Intersection> Sphere::intersect(const Ray& ray) const {
    const auto origin = ray.get_origin();
    const auto direction = ray.get_direction();
    const auto r = center - origin;
    const auto s1 = glm::dot(direction, r);
    const auto d_2 = glm::length(r) * glm::length(r) - s1 * s1;
    if (radius * radius >= d_2) {
        const auto s2 = glm::sqrt(radius * radius - d_2);
        const auto t0 = s1 - s2;
        const auto t1 = s1 + s2;
        if (t0 >= 0) {
            const auto hit_point = origin + direction * t0;
            const auto outward_normal = glm::normalize(hit_point - center);
            const auto [u, v] = get_uv(outward_normal);
            return Intersection(t0, hit_point, outward_normal, material, u, v);
        }
        if (t1 >= 0) {
            const auto hit_point = origin + direction * t1;
            const auto outward_normal = glm::normalize(hit_point - center);
            const auto [u, v] = get_uv(outward_normal);
            return Intersection(t1, hit_point, outward_normal, material, u, v);
        }
    }
    return std::nullopt;
}

AABB Sphere::bounding_box(float /*t0*/, float /*t1*/) const {
    return {center - glm::vec3(radius, radius, radius), center + glm::vec3(radius, radius, radius)};
}

std::pair<float, float> Sphere::get_uv(const glm::vec3& p) const {
    //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
    //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
    //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>
    const auto phi = std::atan2(-p.z, p.x) + static_cast<float>(std::numbers::pi);
    const auto theta = std::acos(-p.y);
    return std::make_pair(phi / (2 * std::numbers::pi), theta / std::numbers::pi);
}
