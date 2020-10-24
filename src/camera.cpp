#include "camera.hpp"

Camera::Camera(const glm::vec3& o, const glm::vec3& ll, const glm::vec3& h, const glm::vec3& v)
    : origin(o)
    , lower_left(ll)
    , horizontal(h)
    , vertical(v) {
}

Ray Camera::get_ray(float u, float v) const {
    return Ray(origin, glm::normalize(lower_left + u * horizontal + v * vertical));
}
