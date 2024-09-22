#include <cmath>

#include <glm/fwd.hpp>
#include <glm/geometric.hpp>

#include "ray.hpp"

Ray::Ray(const glm::vec3& o, const glm::vec3& d, float t)
    : origin(o), direction(d), time(t) {}

glm::vec3 Ray::get_origin() const {
    return origin;
}

glm::vec3 Ray::get_direction() const {
    return direction;
}

float Ray::get_time() const {
    return time;
}

glm::vec3 Ray::reflect(const glm::vec3& n) const {
    return direction - 2.0F * glm::dot(direction, n) * n;
}

glm::vec3 Ray::refract(const glm::vec3& n, float refractive_index) const {
    auto cosi = -glm::dot(n, direction);
    auto n1 = n;
    auto eta = 1.0F / refractive_index;
    if (cosi < 0) {
        cosi *= -1.0F;
        n1 = -n;
        eta = refractive_index / 1.0F;
    }
    auto k = 1.0F - eta * eta * (1.0F - cosi * cosi);
    if (k < 0) {
        return reflect(n1);
    }
    return eta * direction + (eta * cosi - std::sqrt(k)) * n1;
}
