#pragma once

#include <glm/fwd.hpp>

#include "noise/perlin.hpp"
#include "texture.hpp"

namespace raytracer::materials {

class NoiseTexture : public Texture {
  private:
    noise::Perlin noise;
    float scale;

  public:
    explicit NoiseTexture(float scale);
    glm::vec3 value(float u, float v, const glm::vec3& p) const override;
};

} // namespace raytracer::materials
