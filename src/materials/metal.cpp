#include <utility>

#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>

#include "collisions/ray.hpp"
#include "materials/metal.hpp"
#include "utils/utils.hpp"

namespace raytracer::materials {

Metal::Metal(const glm::vec3& a, float f)
    : albedo(a), fuzziness(f) {
}

std::pair<glm::vec3, collisions::Ray> Metal::scatter(const collisions::Ray& ray, const glm::vec3& pos, const glm::vec3& n, float /*u*/, float /*v*/) const {
    const auto reflec = ray.reflect(n);
    const auto scattered = collisions::Ray(pos + OFFSET * n, glm::normalize(reflec + fuzziness * utils::random_in_unit_sphere()), ray.get_time());
    return std::make_pair(albedo, scattered);
}

} // namespace raytracer::materials
