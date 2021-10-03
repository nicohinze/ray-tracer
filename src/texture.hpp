#pragma once

#include <glm/glm.hpp>

class Texture {
  public:
    virtual ~Texture() = default;
    virtual glm::vec3 value(float u, float v, const glm::vec3& p) const = 0;
};
