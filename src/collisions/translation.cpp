#include <memory>
#include <optional>

#include <glm/ext/vector_float3.hpp>

#include "collisions/aabb.hpp"
#include "collisions/hittable.hpp"
#include "collisions/intersection.hpp"
#include "collisions/ray.hpp"
#include "collisions/translation.hpp"

namespace raytracer::collisions {

Translation::Translation(const std::shared_ptr<Hittable>& object, const glm::vec3& offset)
    : object(object)
    , offset(offset)
    , aabb(object->bounding_box(0, 0) + offset) {
}

std::optional<Intersection> Translation::intersect_impl(const Ray& ray) const {
    const auto offset_ray = Ray(ray.get_origin() - offset, ray.get_direction(), ray.get_time());
    const auto intersect = object->intersect(offset_ray);
    if (intersect.has_value()) {
        return Intersection(
            intersect->get_distance(),
            intersect->get_position() + offset,
            intersect->get_normal(),
            intersect->get_material(),
            intersect->get_u(),
            intersect->get_v()
        );
    }
    return std::nullopt;
}

AABB Translation::bounding_box(float /*t0*/, float /*t1*/) const {
    return aabb;
}

} // namespace raytracer::collisions
