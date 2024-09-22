#pragma once

#include "hittable.hpp"
#include "material.hpp"

class GeometryObject : public Hittable {
  protected:
    const Material* material;

  public:
    explicit GeometryObject(const Material* m);
    GeometryObject(const GeometryObject& other) = default;
    GeometryObject(GeometryObject&& other) = delete;
    GeometryObject& operator=(const GeometryObject& other) = default;
    GeometryObject& operator=(GeometryObject&& other) = delete;
    ~GeometryObject() override = default;
    virtual std::pair<float, float> get_uv(const glm::vec3& p) const = 0;
};
