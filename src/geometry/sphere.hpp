#pragma once

#include <optional>

#include <glm/glm.hpp>

#include "collisions/aabb.hpp"
#include "collisions/intersection.hpp"
#include "collisions/ray.hpp"
#include "geometry/geometry_object.hpp"
#include "materials/material.hpp"

namespace raytracer::geometry {

class Sphere : public GeometryObject {
  private:
    glm::vec3 center;
    float radius;

  public:
    Sphere(const glm::vec3& c, float r, const materials::Material* m);
    std::optional<collisions::Intersection> intersect(const collisions::Ray& ray) const override;
    collisions::AABB bounding_box(float t0, float t1) const override;
    std::pair<float, float> get_uv(const glm::vec3& p) const override;
};

} // namespace raytracer::geometry
