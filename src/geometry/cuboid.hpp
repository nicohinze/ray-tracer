#pragma once

#include <memory>
#include <optional>
#include <utility>
#include <vector>

#include <glm/ext/vector_float3.hpp>

#include "collisions/aabb.hpp"
#include "collisions/intersection.hpp"
#include "collisions/ray.hpp"
#include "geometry/geometry_object.hpp"
#include "geometry/quad.hpp"
#include "materials/material.hpp"

namespace raytracer::geometry {

class Cuboid : public GeometryObject {
  private:
    std::vector<std::shared_ptr<Quad>> sides;
    glm::vec3 min;
    glm::vec3 max;

  public:
    Cuboid(const glm::vec3& a, const glm::vec3& b, const materials::Material* m);
    std::optional<collisions::Intersection> intersect_impl(const collisions::Ray& ray) const override;
    collisions::AABB bounding_box(float t0, float t1) const override;
    std::pair<float, float> get_uv(const glm::vec3& p) const override;
};

} // namespace raytracer::geometry
