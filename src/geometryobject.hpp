#pragma once

#include <memory>
#include <optional>

#include "intersection.hpp"
#include "material.hpp"
#include "ray.hpp"

class GeometryObject {
  protected:
    const Material* material;

  public:
    GeometryObject(const Material* m);
    virtual ~GeometryObject() = default;
    virtual std::optional<Intersection> intersect(const Ray& ray) const = 0;
    const Material* get_material() const;
};
