#pragma once

#include <glm/ext/vector_float3.hpp>

#include "materials/material.hpp"

namespace raytracer::collisions {

class Intersection {
  private:
    float distance;
    glm::vec3 position;
    glm::vec3 normal;
    const materials::Material* material;
    float u;
    float v;

  public:
    Intersection(float d, const glm::vec3& p, const glm::vec3& n, const materials::Material* m, float u, float v);
    float get_distance() const;
    glm::vec3 get_position() const;
    glm::vec3 get_normal() const;
    const materials::Material* get_material() const;
    float get_u() const;
    float get_v() const;
};

} // namespace raytracer::collisions
