#pragma once

#include <utility>

#include <glm/ext/vector_float3.hpp>

#include "collisions/ray.hpp"
#include "materials/material.hpp"

namespace raytracer::materials {

class Dielectric : public Material {
  private:
    float refractive_index;

    float schlick_approx(float cosine) const;

  public:
    explicit Dielectric(float r);
    std::pair<glm::vec3, collisions::Ray> scatter(const collisions::Ray& ray, const glm::vec3& pos, const glm::vec3& n, float u, float v) const override;
};

} // namespace raytracer::materials
