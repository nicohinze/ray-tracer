#pragma once

#include <glm/ext/vector_float3.hpp>

#include "texture.hpp"

namespace raytracer::materials {

class SolidColor : public Texture {
  private:
    glm::vec3 color;

  public:
    explicit SolidColor(const glm::vec3& c);
    glm::vec3 value(float u, float v, const glm::vec3& p) const override;
};

} // namespace raytracer::materials
