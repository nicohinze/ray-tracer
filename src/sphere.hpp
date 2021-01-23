#pragma once

#include <optional>

#include <glm/glm.hpp>

#include "geometryobject.hpp"
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
    bool intersect(const Sphere& other) const;
};
