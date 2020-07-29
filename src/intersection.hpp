#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "material.hpp"

class Intersection {
  private:
  public:
    float distance;
    glm::vec3 position;
    glm::vec3 normal;
    const Material* material;

    Intersection(float d, const glm::vec3& p, const glm::vec3& n, const Material* m);
    ~Intersection() = default;
};
