#include <memory>
#include <utility>

#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>

#include "collisions/ray.hpp"
#include "materials/lambertian.hpp"
#include "materials/solid_color.hpp"
#include "materials/texture.hpp"
#include "utils/utils.hpp"

namespace raytracer::materials {

Lambertian::Lambertian(const glm::vec3& a)
    : albedo(std::make_unique<SolidColor>(a)) {}

Lambertian::Lambertian(std::unique_ptr<Texture> a)
    : albedo(std::move(a)) {}

std::pair<glm::vec3, collisions::Ray> Lambertian::scatter(const collisions::Ray& ray, const glm::vec3& pos, const glm::vec3& n, float u, float v) const {
    const auto target = glm::normalize(n + utils::random_in_unit_sphere());
    const auto scattered = collisions::Ray(pos + OFFSET * n, target, ray.get_time());
    return std::make_pair(albedo->value(u, v, pos), scattered);
}

} // namespace raytracer::materials
