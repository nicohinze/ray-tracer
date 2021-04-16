#pragma once

#include "material.hpp"

class Metal : public Material {
  private:
    glm::vec3 albedo;
    float fuzziness;

  public:
    Metal(const glm::vec3& a, float f);
    std::pair<glm::vec3, Ray> scatter(const Ray& ray, const glm::vec3& pos, const glm::vec3& n) const override;
};
