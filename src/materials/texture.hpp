#pragma once

#include <glm/glm.hpp>

namespace raytracer::materials {

class Texture {
  public:
    Texture() = default;
    Texture(const Texture&) = default;
    Texture(Texture&&) = delete;
    Texture& operator=(const Texture&) = default;
    Texture& operator=(Texture&&) = delete;
    virtual ~Texture() = default;
    virtual glm::vec3 value(float u, float v, const glm::vec3& p) const = 0;
};

} // namespace raytracer::materials
