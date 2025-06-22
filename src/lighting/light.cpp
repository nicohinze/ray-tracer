#include <glm/ext/vector_float3.hpp>

#include "light.hpp"

namespace raytracer::lighting {

Light::Light(const glm::vec3& p, float i)
    : position(p), intensity(i) {
}

glm::vec3 Light::get_position() const {
    return position;
}

float Light::get_intensity() const {
    return intensity;
}

} // namespace raytracer::lighting
