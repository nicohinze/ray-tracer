#include "ray.hpp"

Ray::Ray(const glm::vec3& o, const glm::vec3& d)
    : origin(o), direction(d) {}

glm::vec3 Ray::get_origin() const {
    return origin;
}

glm::vec3 Ray::get_direction() const {
    return direction;
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
    return eta * direction + (eta * cosi - std::sqrt(k)) * n1;
}
