#pragma once

#include <utility>

#include <glm/ext/vector_float3.hpp>

#include "collisions/ray.hpp"

namespace raytracer::materials {

class Material {
  public:
    Material() = default;
    Material(const Material&) = default;
    Material(Material&&) = delete;
    Material& operator=(const Material&) = default;
    Material& operator=(Material&&) = delete;
    virtual ~Material() = default;
    virtual std::pair<glm::vec3, collisions::Ray> scatter(const collisions::Ray& ray, const glm::vec3& pos, const glm::vec3& n, float u, float v) const = 0;
    virtual glm::vec3 emit(float u, float v, const glm::vec3& p) const;

  protected:
    static constexpr auto OFFSET = 1e-3F;
};

} // namespace raytracer::materials
