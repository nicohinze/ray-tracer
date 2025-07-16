#pragma once

#include <memory>
#include <optional>

#include <glm/ext/vector_float3.hpp>

#include "collisions/aabb.hpp"
#include "collisions/hittable.hpp"
#include "collisions/intersection.hpp"
#include "collisions/ray.hpp"
#include "materials/material.hpp"
#include "materials/texture.hpp"

namespace raytracer::collisions {

class ConstantMedium : public Hittable {
  private:
    float neg_inv_density;
    std::shared_ptr<Hittable> boundary;
    std::shared_ptr<materials::Material> phase_function;

  public:
    ConstantMedium(std::shared_ptr<Hittable> boundary, float density, const std::shared_ptr<materials::Texture>& tex);
    ConstantMedium(std::shared_ptr<Hittable> boundary, float density, const glm::vec3& albedo);
    std::optional<Intersection> intersect_impl(const Ray& ray) const override;
    AABB bounding_box(float t0, float t1) const override;
};

} // namespace raytracer::collisions
