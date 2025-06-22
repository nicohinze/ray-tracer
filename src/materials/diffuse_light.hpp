#pragma once

#include <memory>
#include <utility>

#include <glm/ext/vector_float3.hpp>

#include "collisions/ray.hpp"
#include "materials/material.hpp"
#include "materials/texture.hpp"

namespace raytracer::materials {

class DiffuseLight : public Material {
  private:
    std::unique_ptr<Texture> tex;

  public:
    explicit DiffuseLight(std::unique_ptr<Texture> tex);
    explicit DiffuseLight(const glm::vec3& emit);

    std::pair<glm::vec3, collisions::Ray> scatter(const collisions::Ray& ray, const glm::vec3& pos, const glm::vec3& n, float u, float v) const override;
    glm::vec3 emit(float u, float v, const glm::vec3& p) const override;
};

} // namespace raytracer::materials
