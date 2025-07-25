#include <glm/ext/vector_float3.hpp>

#include "solid_color.hpp"

namespace raytracer::materials {

SolidColor::SolidColor(const glm::vec3& c)
    : color(c) {}

glm::vec3 SolidColor::value(float /*u*/, float /*v*/, const glm::vec3& /*p*/) const {
    return color;
}

} // namespace raytracer::materials
