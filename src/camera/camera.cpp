#include <cmath>
#include <numbers>

#include <glm/geometric.hpp>

#include "camera/camera.hpp"
#include "collisions/ray.hpp"
#include "utils/utils.hpp"

namespace raytracer::camera {

Camera::Camera() // NOLINT(cppcoreguidelines-pro-type-member-init,hicpp-member-init)
    : origin(glm::vec3(0, 0, 0))
    , u(glm::vec3(1, 0, 0))
    , v(glm::vec3(0, 1, 0))
    , w(glm::vec3(0, 0, 1))
    , bg({})
    , lens_radius(0) {
    auto viewport_height = 2.0F;
    auto viewport_width = 4.0F / 3.0F * viewport_height;
    horizontal = glm::vec3(viewport_width, 0, 0);
    vertical = glm::vec3(0, viewport_height, 0);
    lower_left = glm::vec3(-viewport_width / 2.0F, -viewport_height / 2.0F, -1);
}

Camera::Camera(const glm::vec3& o, const glm::vec3& lookat, const glm::vec3& vup, const glm::vec3& bg, float vfov, float aspect_ratio, float aperture, float focus_dist, float t1, float t2) // NOLINT(cppcoreguidelines-pro-type-member-init,hicpp-member-init)
    : origin(o)
    , bg(bg)
    , lens_radius(aperture / 2.0F)
    , time1(t1)
    , time2(t2) {
    auto theta = static_cast<float>(std::numbers::pi) / 180.0F * vfov;
    auto h = std::tan(theta / 2.0F);
    auto viewport_height = 2.0F * h;
    auto viewport_width = aspect_ratio * viewport_height;
    w = glm::normalize(origin - lookat);
    u = glm::normalize(glm::cross(vup, w));
    v = glm::cross(w, u);
    horizontal = focus_dist * viewport_width * u;
    vertical = focus_dist * viewport_height * v;
    lower_left = origin - horizontal / 2.0F - vertical / 2.0F - focus_dist * w;
}

collisions::Ray Camera::get_ray(float x, float y) const {
    auto rng = lens_radius * utils::random_in_unit_disk();
    auto offset = u * rng.x + v * rng.y;
    return {origin + offset, glm::normalize(lower_left + x * horizontal + y * vertical - origin - offset), utils::random_float(time1, time2)};
}

glm::vec3 Camera::get_bg() const {
    return bg;
}

} // namespace raytracer::camera
