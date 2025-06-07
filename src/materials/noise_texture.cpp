#include <cmath>

#include "glm/fwd.hpp"
#include "noise_texture.hpp"

namespace raytracer::materials {

NoiseTexture::NoiseTexture(float scale)
    : scale(scale) {
}

glm::vec3 NoiseTexture::value(float /*u*/, float /*v*/, const glm::vec3& p) const {
    return (1 + std::sin(scale * p.z + 10 * noise.turbulence(p, 7))) * glm::vec3(.5, .5, .5);
}

} // namespace raytracer::materials
