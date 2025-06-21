#include <glm/fwd.hpp>
#include <memory>
#include <utility>

#include "collisions/ray.hpp"
#include "materials/diffuse_light.hpp"
#include "materials/solid_color.hpp"
#include "materials/texture.hpp"

namespace raytracer::materials {

DiffuseLight::DiffuseLight(std::unique_ptr<Texture> tex)
    : tex(std::move(tex)) {
}

DiffuseLight::DiffuseLight(const glm::vec3& emit)
    : tex(std::make_unique<SolidColor>(emit)) {
}

std::pair<glm::vec3, collisions::Ray> DiffuseLight::scatter(const collisions::Ray& /*ray*/, const glm::vec3& pos, const glm::vec3& /*n*/, float /*u*/, float /*v*/) const {
    return std::make_pair(glm::vec3(0, 0, 0), collisions::Ray(pos, glm::vec3(0, 0, 0)));
}

glm::vec3 DiffuseLight::emit(float u, float v, const glm::vec3& p) const {
    return tex->value(u, v, p);
}

} // namespace raytracer::materials
