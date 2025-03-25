#include <algorithm>
#include <limits>

#include "aabb.hpp"
#include "ray.hpp"

namespace raytracer::collisions {

AABB::AABB()
    : min(glm::vec3(0, 0, 0)), max(glm::vec3(0, 0, 0)) {
}
AABB::AABB(const glm::vec3& min, const glm::vec3& max)
    : min(min), max(max) {}

bool AABB::intersect(const Ray& ray) const {
    float t_min = 0;
    float t_max = std::numeric_limits<float>::max();
    for (auto i = 0; i < 3; ++i) {
        const auto inv_dir = 1.0F / ray.get_direction()[i];
        auto t0 = (min[i] - ray.get_origin()[i]) * inv_dir;
        auto t1 = (max[i] - ray.get_origin()[i]) * inv_dir;
        if (inv_dir < 0.0F) {
            std::swap(t0, t1);
        }
        t_min = std::max(t0, t_min);
        t_max = std::min(t1, t_max);
        if (t_min >= t_max) {
            return false;
        }
    }
    return true;
}

glm::vec3 AABB::get_min() const {
    return min;
}

glm::vec3 AABB::get_max() const {
    return max;
}

AABB surrounding_box(const AABB& aabb0, const AABB& aabb1) {
    const auto p0 = glm::vec3(
        std::min(aabb0.get_min().x, aabb1.get_min().x),
        std::min(aabb0.get_min().y, aabb1.get_min().y),
        std::min(aabb0.get_min().z, aabb1.get_min().z)
    );
    const auto p1 = glm::vec3(
        std::max(aabb0.get_max().x, aabb1.get_max().x),
        std::max(aabb0.get_max().y, aabb1.get_max().y),
        std::max(aabb0.get_max().z, aabb1.get_max().z)
    );
    return {p0, p1};
}

} // namespace raytracer::collisions
