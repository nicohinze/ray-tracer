#pragma once

#include "hittable.hpp"
#include "material.hpp"

class GeometryObject : public Hittable {
  protected:
    const Material* material;

  public:
    explicit GeometryObject(const Material* m);
    GeometryObject(const GeometryObject& other) = default;
    GeometryObject(GeometryObject&& other) = default;
    GeometryObject& operator=(const GeometryObject& other) = default;
    GeometryObject& operator=(GeometryObject&& other) = default;
    virtual ~GeometryObject() = default;
};
