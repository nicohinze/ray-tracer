#include <iostream>
#include <optional>

#include <glm/glm.hpp>

#include "geometryobject.hpp"
#include "intersection.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "sphere.hpp"

Sphere::Sphere(const glm::vec3& c, float r, const Material* m)
    : GeometryObject(m), center(c), radius(r) {}

std::optional<Intersection> Sphere::intersect(const Ray& ray) const {
    auto v = center - ray.get_origin();
    auto u1 = glm::dot(ray.get_direction(), v);
    auto d_2 = glm::length(v) * glm::length(v) - u1 * u1;
    if (radius * radius >= d_2) {
        auto u2 = glm::sqrt(radius * radius - d_2);
        auto t0 = u1 - u2;
        auto t1 = u1 + u2;
        if (t0 >= 0) {
            return Intersection(t0, ray.get_direction() * t0, glm::normalize(ray.get_direction() * t0 - center), material);
        }
        if (t1 >= 0) {
            return Intersection(t1, ray.get_direction() * t1, glm::normalize(ray.get_direction() * t1 - center), material);
        }
    }
    return std::nullopt;
}
