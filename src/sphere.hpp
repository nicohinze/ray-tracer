#pragma once

#include <optional>

#include <glm/glm.hpp>

#include "aabb.hpp"
#include "geometry_object.hpp"
#include "intersection.hpp"
#include "material.hpp"
#include "ray.hpp"

class Sphere : public GeometryObject {
  private:
    glm::vec3 center;
    float radius;

  public:
    Sphere(const glm::vec3& c, float r, const Material* m);
    std::optional<Intersection> intersect(const Ray& ray) const override;
    std::optional<AABB> bounding_box(float t0, float t1) const override;
    std::pair<float, float> get_uv(const glm::vec3& p) const override;
};
