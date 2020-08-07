#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "material.hpp"

class Intersection {
  private:
    float distance;
    glm::vec3 position;
    glm::vec3 normal;
    const Material* material;

  public:
    Intersection(float d, const glm::vec3& p, const glm::vec3& n, const Material* m);
    float get_distance() const;
    glm::vec3 get_position() const;
    glm::vec3 get_normal() const;
    const Material* get_material() const;
};
