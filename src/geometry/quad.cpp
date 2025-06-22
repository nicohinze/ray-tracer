#include <algorithm>
#include <cmath>
#include <optional>
#include <utility>

#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>

#include "collisions/aabb.hpp"
#include "collisions/intersection.hpp"
#include "collisions/ray.hpp"
#include "geometry/quad.hpp"
#include "geometry_object.hpp"
#include "materials/material.hpp"

namespace raytracer::geometry {

Quad::Quad(const glm::vec3& p, const glm::vec3& u, const glm::vec3& v, const materials::Material* m)
    : GeometryObject(m)
    , corner(p)
    , u(u)
    , v(v)
    , n(glm::normalize(glm::cross(u, v)))
    , w(n / glm::dot(n, n))
    , d(glm::dot(n, corner)) {
    const auto q = p + u;
    const auto r = p + v;
    const auto s = p + u + v;
    auto min = glm::vec3();
    auto max = glm::vec3();
    min.x = std::min({p.x, q.x, r.x, s.x});
    min.y = std::min({p.y, q.y, r.y, s.y});
    min.z = std::min({p.z, q.z, r.z, s.z});
    max.x = std::max({p.x, q.x, r.x, s.x});
    max.y = std::max({p.y, q.y, r.y, s.y});
    max.z = std::max({p.z, q.z, r.z, s.z});
    aabb = collisions::AABB(min, max);
}

std::optional<collisions::Intersection> Quad::intersect_impl(const collisions::Ray& ray) const {
    const auto direction = ray.get_direction();
    const auto denominator = glm::dot(n, direction);
    if (std::fabs(denominator) < 1e-8f) {
        return std::nullopt;
    }
    const auto t = (d - glm::dot(n, ray.get_origin())) / denominator;
    if (t < 0) {
        return std::nullopt;
    }
    const auto hit_point = ray.get_origin() + t * direction;
    const auto planar_vec = hit_point - corner;
    // const auto alpha = glm::dot(w, glm::cross(planar_vec, v));
    // const auto beta = glm::dot(w, glm::cross(u, planar_vec));
    const auto alpha = glm::dot(planar_vec, u) / glm::dot(u, u);
    const auto beta = glm::dot(planar_vec, v) / glm::dot(v, v);
    if (0 <= alpha && alpha <= 1 && 0 <= beta && beta <= 1) {
        return collisions::Intersection(t, hit_point, glm::dot(n, direction) < 0 ? n : -n, material, alpha, beta);
    }
    return std::nullopt;
}

collisions::AABB Quad::bounding_box(float /*t0*/, float /*t1*/) const {
    return aabb;
}

std::pair<float, float> Quad::get_uv(const glm::vec3& p) const {
    const auto planar_vec = p - corner;
    const auto alpha = glm::dot(w, glm::cross(planar_vec, v));
    const auto beta = glm::dot(w, glm::cross(u, planar_vec));
    return std::make_pair(alpha, beta);
}

} // namespace raytracer::geometry
