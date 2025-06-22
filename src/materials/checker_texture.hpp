#pragma once

#include <memory>

#include <glm/ext/vector_float3.hpp>

#include "materials/texture.hpp"

namespace raytracer::materials {

class CheckerTexture : public Texture {
  private:
    std::shared_ptr<Texture> odd;
    std::shared_ptr<Texture> even;

  public:
    CheckerTexture(const glm::vec3& o, const glm::vec3& e);
    CheckerTexture(std::shared_ptr<Texture> o, std::shared_ptr<Texture> e);
    glm::vec3 value(float u, float v, const glm::vec3& p) const override;
};

} // namespace raytracer::materials
