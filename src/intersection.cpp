#include <memory>

#include "intersection.hpp"
#include "material.hpp"

Intersection::Intersection(float d, const glm::vec3& p, const glm::vec3& n, const Material* m)
    : distance(d), position(p), normal(n), material(m) {}

float Intersection::get_distance() const {
    return distance;
}
glm::vec3 Intersection::get_position() const {
    return position;
}
glm::vec3 Intersection::get_normal() const {
    return normal;
}
const Material* Intersection::get_material() const {
    return material;
}
