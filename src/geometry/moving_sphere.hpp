#pragma once

#include <optional>

#include <glm/glm.hpp>

#include "collisions/aabb.hpp"
#include "collisions/intersection.hpp"
#include "collisions/ray.hpp"
#include "geometry/geometry_object.hpp"
#include "materials/material.hpp"

namespace raytracer::geometry {

class MovingSphere : public GeometryObject {
  private:
    glm::vec3 center1;
    glm::vec3 center2;
    float radius;
    float time1;
    float time2;

  public:
    MovingSphere(glm::vec3 c1, glm::vec3 c2, float r, float t1, float t2, const materials::Material* m);
    glm::vec3 get_center(float time) const;
    std::optional<collisions::Intersection> intersect_impl(const collisions::Ray& ray) const override;
    collisions::AABB bounding_box(float t0, float t1) const override;
    std::pair<float, float> get_uv(const glm::vec3& p) const override;
};

} // namespace raytracer::geometry
