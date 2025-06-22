#pragma once

#include <memory>
#include <utility>

#include <glm/ext/vector_float3.hpp>

#include "collisions/ray.hpp"
#include "materials/material.hpp"
#include "materials/texture.hpp"

namespace raytracer::materials {

class Lambertian : public Material {
  private:
    std::unique_ptr<Texture> albedo;

  public:
    Lambertian(const Lambertian&) = delete;
    Lambertian(Lambertian&&) = delete;
    Lambertian& operator=(const Lambertian&) = delete;
    Lambertian& operator=(Lambertian&&) = delete;
    ~Lambertian() override = default;
    explicit Lambertian(const glm::vec3& a);
    explicit Lambertian(std::unique_ptr<Texture> a);
    std::pair<glm::vec3, collisions::Ray> scatter(const collisions::Ray& ray, const glm::vec3& pos, const glm::vec3& n, float u, float v) const override;
};

} // namespace raytracer::materials
