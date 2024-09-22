#include <glm/fwd.hpp>

#include "solid_color.hpp"

SolidColor::SolidColor(const glm::vec3& c)
    : color(c) {}

glm::vec3 SolidColor::value(float /*u*/, float /*v*/, const glm::vec3& /*p*/) const {
    return color;
}
