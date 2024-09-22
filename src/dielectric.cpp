#include <cmath>
#include <utility>

#include <glm/fwd.hpp>
#include <glm/geometric.hpp>

#include "dielectric.hpp"
#include "ray.hpp"
#include "utils.hpp"

Dielectric::Dielectric(float r)
    : refractive_index(r) {
}

float Dielectric::schlick_approx(float cosine) const {
    auto r0 = (1 - refractive_index) / (1 + refractive_index);
    r0 *= r0;
    return r0 + (1 - r0) * std::pow(1 - cosine, 5.0F); // NOLINT(readability-magic-numbers)
}

std::pair<glm::vec3, Ray> Dielectric::scatter(const Ray& ray, const glm::vec3& pos, const glm::vec3& n, float /*u*/, float /*v*/) const {
    const auto scattered = [this, &ray, &pos, &n] {
        const auto cosine = -glm::dot(n, ray.get_direction());
        if (random_float() < schlick_approx(cosine < 0 ? -cosine : cosine)) {
            const auto k = glm::dot(ray.get_direction(), n) < 0 ? 1.0F : -1.0F;
            return Ray(pos + k * OFFSET * n, ray.reflect(n), ray.get_time());
        }
        const auto k = glm::dot(ray.get_direction(), n) > 0 ? 1.0F : -1.0F;
        return Ray(pos + k * OFFSET * n, ray.refract(n, refractive_index), ray.get_time());
    }();
    return std::make_pair(glm::vec3(1, 1, 1), scattered);
}
