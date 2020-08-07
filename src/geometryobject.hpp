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
    explicit GeometryObject(const Material* m);
    GeometryObject(const GeometryObject& other) = default;
    GeometryObject(GeometryObject&& other) = default;
    GeometryObject& operator=(const GeometryObject& other) = default;
    GeometryObject& operator=(GeometryObject&& other) = default;
    virtual ~GeometryObject() = default;
    virtual std::optional<Intersection> intersect(const Ray& ray) const = 0;
    const Material* get_material() const;
};
