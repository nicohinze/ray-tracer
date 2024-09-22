#include <memory>
#include <utility>

#include <glm/fwd.hpp>
#include <glm/geometric.hpp>

#include "lambertian.hpp"
#include "ray.hpp"
#include "solid_color.hpp"
#include "texture.hpp"
#include "utils.hpp"

Lambertian::Lambertian(const glm::vec3& a)
    : albedo(std::make_unique<SolidColor>(a)) {}

Lambertian::Lambertian(std::unique_ptr<Texture> a)
    : albedo(std::move(a)) {}

std::pair<glm::vec3, Ray> Lambertian::scatter(const Ray& ray, const glm::vec3& pos, const glm::vec3& n, float u, float v) const {
    const auto target = glm::normalize(n + random_in_unit_sphere());
    const auto scattered = Ray(pos + OFFSET * n, target, ray.get_time());
    return std::make_pair(albedo->value(u, v, pos), scattered);
}
