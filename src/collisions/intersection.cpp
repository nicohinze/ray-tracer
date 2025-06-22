#include <glm/ext/vector_float3.hpp>

#include "collisions/intersection.hpp"
#include "materials/material.hpp"

namespace raytracer::collisions {

Intersection::Intersection(float d, const glm::vec3& p, const glm::vec3& n, const materials::Material* m, float u, float v)
    : distance(d), position(p), normal(n), material(m), u(u), v(v) {}

float Intersection::get_distance() const {
    return distance;
}

glm::vec3 Intersection::get_position() const {
    return position;
}

glm::vec3 Intersection::get_normal() const {
    return normal;
}

const materials::Material* Intersection::get_material() const {
    return material;
}

float Intersection::get_u() const {
    return u;
}

float Intersection::get_v() const {
    return v;
}

} // namespace raytracer::collisions
