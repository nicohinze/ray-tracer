#include <memory>
#include <utility>

#include "collisions/ray.hpp"
#include "materials/isotropic.hpp"
#include "materials/solid_color.hpp"
#include "materials/texture.hpp"
#include "utils/utils.hpp"
#include <glm/ext/vector_float3.hpp>

namespace raytracer::materials {

Isotropic::Isotropic(const glm::vec3& albedo)
    : tex(std::make_shared<SolidColor>(albedo)) {
}

Isotropic::Isotropic(std::shared_ptr<Texture> tex)
    : tex(std::move(tex)) {
}

std::pair<glm::vec3, collisions::Ray> Isotropic::scatter(const collisions::Ray& ray, const glm::vec3& pos, const glm::vec3& /*n*/, float u, float v) const {
    return std::make_pair(
        tex->value(u, v, pos),
        collisions::Ray(pos, utils::random_in_unit_sphere(), ray.get_time())
    );
}

} // namespace raytracer::materials
