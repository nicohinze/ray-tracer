#pragma once

#include "materials/material.hpp"

namespace raytracer::materials {

class Metal : public Material {
  private:
    glm::vec3 albedo;
    float fuzziness;

  public:
    Metal(const glm::vec3& a, float f);
    std::pair<glm::vec3, collisions::Ray> scatter(const collisions::Ray& ray, const glm::vec3& pos, const glm::vec3& n, float u, float v) const override;
};

} // namespace raytracer::materials
