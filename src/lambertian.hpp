#pragma once

#include "material.hpp"

class Lambertian : public Material {
  private:
    glm::vec3 albedo;

  public:
    explicit Lambertian(const glm::vec3& a);
    ~Lambertian() override = default;
    std::pair<glm::vec3, Ray> scatter(const Ray& ray, const glm::vec3& pos, const glm::vec3& n) const override;
};
