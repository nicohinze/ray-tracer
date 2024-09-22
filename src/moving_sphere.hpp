#pragma once

#include <optional>

#include <glm/glm.hpp>

#include "aabb.hpp"
#include "geometry_object.hpp"
#include "intersection.hpp"
#include "material.hpp"
#include "ray.hpp"

class MovingSphere : public GeometryObject {
  private:
    glm::vec3 center1;
    glm::vec3 center2;
    float radius;
    float time1;
    float time2;

  public:
    MovingSphere(glm::vec3 c1, glm::vec3 c2, float r, float t1, float t2, const Material* m);
    glm::vec3 get_center(float time) const;
    std::optional<Intersection> intersect(const Ray& ray) const override;
    AABB bounding_box(float t0, float t1) const override;
    std::pair<float, float> get_uv(const glm::vec3& p) const override;
};
