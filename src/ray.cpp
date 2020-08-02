#include "ray.hpp"

Ray::Ray(const glm::vec3& o, const glm::vec3& d)
    : origin(o), direction(d) {}

glm::vec3 Ray::reflect(const glm::vec3& n) const {
    return direction - 2.0F * glm::dot(direction, n) * n;
}