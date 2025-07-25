#pragma once

#include <optional>
#include <utility>

#include <glm/ext/vector_float3.hpp>

#include "collisions/aabb.hpp"
#include "collisions/intersection.hpp"
#include "collisions/ray.hpp"
#include "geometry/geometry_object.hpp"
#include "materials/material.hpp"

namespace raytracer::geometry {

class Quad : public GeometryObject {
  private:
    glm::vec3 corner;
    glm::vec3 u;
    glm::vec3 v;
    glm::vec3 n;
    glm::vec3 w;
    float d;
    collisions::AABB aabb;

  public:
    Quad(const glm::vec3& p, const glm::vec3& u, const glm::vec3& v, const materials::Material* m);
    std::optional<collisions::Intersection> intersect_impl(const collisions::Ray& ray) const override;
    collisions::AABB bounding_box(float t0, float t1) const override;
    std::pair<float, float> get_uv(const glm::vec3& p) const override;
    glm::vec3 get_corner();
    glm::vec3 get_n();
};

} // namespace raytracer::geometry
