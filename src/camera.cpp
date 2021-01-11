#include <cmath>

#include "camera.hpp"
#include "utils.hpp"

Camera::Camera() // NOLINT(cppcoreguidelines-pro-type-member-init,hicpp-member-init)
    : origin(glm::vec3(0, 0, 0))
    , u(glm::vec3(1, 0, 0))
    , v(glm::vec3(0, 1, 0))
    , w(glm::vec3(0, 0, 1))
    , lens_radius(0) {
    auto viewport_height = 2.0F;
    auto viewport_width = 4.0F / 3.0F * viewport_height; // NOLINT(readability-magic-numbers)
    horizontal = glm::vec3(viewport_width, 0, 0);
    vertical = glm::vec3(0, viewport_height, 0);
    lower_left = glm::vec3(-viewport_width / 2.0F, -viewport_height / 2.0F, -1);
}

Camera::Camera(const glm::vec3& o, const glm::vec3& lookat, const glm::vec3& vup, float vfov, float aspect_ratio, float aperture, float focus_dist) // NOLINT(cppcoreguidelines-pro-type-member-init,hicpp-member-init)
    : origin(o) {
    auto theta = static_cast<float>(M_PI) / 180.0F * vfov; // NOLINT(readability-magic-numbers)
    auto h = std::tan(theta / 2.0F);
    auto viewport_height = 2.0F * h;
    auto viewport_width = aspect_ratio * viewport_height;
    w = glm::normalize(origin - lookat);
    u = glm::normalize(glm::cross(vup, w));
    v = glm::cross(w, u);
    horizontal = focus_dist * viewport_width * u;
    vertical = focus_dist * viewport_height * v;
    lower_left = origin - horizontal / 2.0F - vertical / 2.0F - focus_dist * w;
    lens_radius = aperture / 2.0F;
}

Ray Camera::get_ray(float x, float y) const {
    auto rng = lens_radius * random_in_unit_disk();
    auto offset = u * rng.x + v * rng.y;
    return Ray(origin + offset, glm::normalize(lower_left + x * horizontal + y * vertical - origin - offset));
}
