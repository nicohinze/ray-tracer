#pragma once

#include "material.hpp"

class Dielectric : public Material {
  private:
    float refractive_index;

    float schlick_approx(float cosine) const;

  public:
    Dielectric(float r);
    std::pair<glm::vec3, Ray> scatter(const Ray& ray, const glm::vec3& pos, const glm::vec3& n) const override;
};
