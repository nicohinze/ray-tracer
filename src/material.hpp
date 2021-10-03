#pragma once

#include <utility>

#include <glm/glm.hpp>

#include "ray.hpp"

class Material {
  public:
    virtual ~Material() = default;
    virtual std::pair<glm::vec3, Ray> scatter(const Ray& ray, const glm::vec3& pos, const glm::vec3& n, float u, float v) const = 0;

  protected:
    static constexpr auto OFFSET = 1e-3F;
};
