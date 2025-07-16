#pragma once

#include <memory>
#include <utility>

#include <glm/ext/vector_float3.hpp>

#include "collisions/ray.hpp"
#include "materials/material.hpp"
#include "materials/texture.hpp"

namespace raytracer::materials {

class Isotropic : public Material {
  private:
    std::shared_ptr<Texture> tex;

  public:
    explicit Isotropic(const glm::vec3& albedo);
    explicit Isotropic(std::shared_ptr<Texture> tex);
    std::pair<glm::vec3, collisions::Ray> scatter(const collisions::Ray& ray, const glm::vec3& pos, const glm::vec3& n, float u, float v) const override;
};

} // namespace raytracer::materials
