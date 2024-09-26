#pragma once

#include "collisions/hittable.hpp"
#include "materials/material.hpp"

namespace raytracer::geometry {

class GeometryObject : public collisions::Hittable {
  protected:
    const materials::Material* material;

  public:
    explicit GeometryObject(const materials::Material* m);
    GeometryObject(const GeometryObject& other) = default;
    GeometryObject(GeometryObject&& other) = delete;
    GeometryObject& operator=(const GeometryObject& other) = default;
    GeometryObject& operator=(GeometryObject&& other) = delete;
    ~GeometryObject() override = default;
    virtual std::pair<float, float> get_uv(const glm::vec3& p) const = 0;
};

} // namespace raytracer::geometry
