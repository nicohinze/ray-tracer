
#include <cmath>
#include <memory>
#include <optional>
#include <utility>
#include <vector>

#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>

#include "collisions/aabb.hpp"
#include "collisions/intersection.hpp"
#include "collisions/ray.hpp"
#include "geometry/cuboid.hpp"
#include "geometry/geometry_object.hpp"
#include "geometry/quad.hpp"
#include "materials/material.hpp"

namespace raytracer::geometry {

Cuboid::Cuboid(const glm::vec3& a, const glm::vec3& b, const materials::Material* m)
    : GeometryObject(m)
    , min(glm::vec3(std::fmin(a.x, b.x), std::fmin(a.y, b.y), std::fmin(a.z, b.z)))
    , max(glm::vec3(std::fmax(a.x, b.x), std::fmax(a.y, b.y), std::fmax(a.z, b.z))) {
    const auto dx = glm::vec3(max.x - min.x, 0, 0);
    const auto dy = glm::vec3(0, max.y - min.y, 0);
    const auto dz = glm::vec3(0, 0, max.z - min.z);
    sides.emplace_back(std::make_shared<Quad>(glm::vec3(min.x, min.y, max.z), dx, dy, m));  // Front
    sides.emplace_back(std::make_shared<Quad>(glm::vec3(max.x, min.y, min.z), -dx, dy, m)); // Back
    sides.emplace_back(std::make_shared<Quad>(glm::vec3(min.x, max.y, max.z), dx, -dz, m)); // Top
    sides.emplace_back(std::make_shared<Quad>(glm::vec3(max.x, min.y, min.z), -dx, dz, m)); // Bottom
    sides.emplace_back(std::make_shared<Quad>(glm::vec3(min.x, min.y, min.z), dz, dy, m));  // Left
    sides.emplace_back(std::make_shared<Quad>(glm::vec3(max.x, min.y, max.z), -dz, dy, m)); // Right
}

std::optional<collisions::Intersection> Cuboid::intersect_impl(const collisions::Ray& ray) const {
    std::optional<collisions::Intersection> closest_intersection = std::nullopt;
    for (const auto& side : sides) {
        if (const auto intersection = side->intersect(ray)) {
            if (!closest_intersection.has_value() || intersection->get_distance() < closest_intersection->get_distance()) {
                closest_intersection = intersection;
            }
        }
    }
    return closest_intersection;
}

collisions::AABB Cuboid::bounding_box(float /*t0*/, float /*t1*/) const {
    return {min, max};
}

std::pair<float, float> Cuboid::get_uv(const glm::vec3& p) const {
    for (const auto& side : sides) {
        if (glm::dot(side->get_n(), (side->get_corner() - p)) < 1e-6f) {
            return side->get_uv(p);
        }
    }
    return {0, 0};
}

} // namespace raytracer::geometry
