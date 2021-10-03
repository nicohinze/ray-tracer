#pragma once

#include <memory>

#include "material.hpp"
#include "texture.hpp"

class Lambertian : public Material {
  private:
    std::unique_ptr<Texture> albedo;

  public:
    explicit Lambertian(const glm::vec3& a);
    explicit Lambertian(std::unique_ptr<Texture> a);
    ~Lambertian() override = default;
    std::pair<glm::vec3, Ray> scatter(const Ray& ray, const glm::vec3& pos, const glm::vec3& n, float u, float v) const override;
};
