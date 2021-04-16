#include "metal.hpp"
#include "utils.hpp"

Metal::Metal(const glm::vec3& a, float f)
    : albedo(a), fuzziness(f) {
}

std::pair<glm::vec3, Ray> Metal::scatter(const Ray& ray, const glm::vec3& pos, const glm::vec3& n) const {
    const auto reflec = ray.reflect(n);
    const auto scattered = Ray(pos + OFFSET * n, glm::normalize(reflec + fuzziness * random_in_unit_sphere()));
    return std::make_pair(albedo, scattered);
}
