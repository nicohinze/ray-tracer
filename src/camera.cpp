#include <cmath>

#include "camera.hpp"

Camera::Camera()
    : origin(glm::vec3(0, 0, 0)) {
    auto viewport_height = 2.0F;
    auto viewport_width = 4.0F / 3.0F * viewport_height;
    horizontal = glm::vec3(viewport_width, 0, 0);
    vertical = glm::vec3(0, viewport_height, 0);
    lower_left = glm::vec3(-viewport_width / 2.0F, -viewport_height / 2.0F, -1);
}

Camera::Camera(const glm::vec3& o, const glm::vec3& lookat, const glm::vec3& vup, float vfov, float aspect_ratio)
    : origin(o) {
    auto theta = static_cast<float>(M_PI) / 180.0F * vfov;
    auto h = std::tan(theta / 2.0F);
    auto viewport_height = 2.0F * h;
    auto viewport_width = aspect_ratio * viewport_height;
    auto w = glm::normalize(origin - lookat);
    auto u = glm::normalize(glm::cross(vup, w));
    auto v = glm::cross(w, u);
    horizontal = viewport_width * u;
    vertical = viewport_height * v;
    lower_left = origin - horizontal / 2.0F - vertical / 2.0F - w;
}

Ray Camera::get_ray(float x, float y) const {
    return Ray(origin, glm::normalize(lower_left + x * horizontal + y * vertical - origin));
}
