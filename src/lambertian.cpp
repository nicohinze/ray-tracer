#include "lambertian.hpp"
#include "utils.hpp"

Lambertian::Lambertian(const glm::vec3& a)
    : albedo(a) {}

std::pair<glm::vec3, Ray> Lambertian::scatter(const Ray& /*ray*/, const glm::vec3& pos, const glm::vec3& n) const {
    const auto target = glm::normalize(n + random_in_unit_sphere());
    const auto scattered = Ray(pos + OFFSET * n, target);
    return std::make_pair(albedo, scattered);
}
