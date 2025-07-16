#include <algorithm>
#include <cmath>
#include <memory>
#include <optional>

#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <utility>

#include "collisions/aabb.hpp"
#include "collisions/constant_medium.hpp"
#include "collisions/hittable.hpp"
#include "collisions/intersection.hpp"
#include "collisions/ray.hpp"
#include "materials/isotropic.hpp"
#include "materials/texture.hpp"
#include "utils/utils.hpp"

namespace raytracer::collisions {

ConstantMedium::ConstantMedium(std::shared_ptr<Hittable> boundary, float density, const std::shared_ptr<materials::Texture>& tex)
    : neg_inv_density(-1 / std::fabs(density)), boundary(std::move(boundary)), phase_function(std::make_shared<materials::Isotropic>(tex)) {
}

ConstantMedium::ConstantMedium(std::shared_ptr<Hittable> boundary, float density, const glm::vec3& albedo)
    : neg_inv_density(-1 / std::fabs(density)), boundary(std::move(boundary)), phase_function(std::make_shared<materials::Isotropic>(albedo)) {
}

std::optional<Intersection> ConstantMedium::intersect_impl(const Ray& ray) const {
    const auto hit1 = boundary->intersect(ray);
    if (!hit1.has_value()) {
        return std::nullopt;
    }
    const auto hit2 = boundary->intersect(Ray(hit1->get_position() + 1e-3f * ray.get_direction(), ray.get_direction()));
    if (!hit2.has_value()) {
        return std::nullopt;
    }
    auto t1 = hit1->get_distance();
    auto t2 = hit2->get_distance();
    if (t1 > t2) {
        std::swap(t1, t2);
    }
    t1 = std::max(t1, 0.0f);
    const auto ray_length = glm::length(ray.get_direction());
    const auto distance_inside_boundary = (t2 - t1) * ray_length;
    const auto hit_distance = neg_inv_density * std::log(std::max(utils::random_float(), 1e-6f));
    if (hit_distance > distance_inside_boundary) {
        return std::nullopt;
    }
    const auto dist = t1 + hit_distance / ray_length;
    return Intersection(
        dist,
        ray.get_origin() + dist * ray.get_direction(),
        glm::vec3(1, 0, 0), // arbitrary
        phase_function.get(),
        0,
        0
    );
}

AABB ConstantMedium::bounding_box(float t0, float t1) const {
    return boundary->bounding_box(t0, t1);
}

} // namespace raytracer::collisions
