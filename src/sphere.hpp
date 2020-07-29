#pragma once

#include <optional>

#include <glm/glm.hpp>

#include "geometryobject.hpp"
#include "intersection.hpp"
#include "material.hpp"
#include "ray.hpp"

class Sphere : public GeometryObject {
  private:
  public:
    glm::vec3 center;
    float radius;

    Sphere(const glm::vec3& c, float r, const Material* m);
    ~Sphere() = default;
    std::optional<Intersection> intersect(const Ray& ray) const override;
};
