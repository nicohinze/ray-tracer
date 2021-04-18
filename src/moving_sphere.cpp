#include "moving_sphere.hpp"

MovingSphere::MovingSphere(const glm::vec3 c1, const glm::vec3 c2, float r, float t1, float t2, const Material* m)
    : GeometryObject(m), center1(c1), center2(c2), radius(r), time1(t1), time2(t2) {}

glm::vec3 MovingSphere::get_center(float time) const {
    return center1 + ((time - time1) / (time2 - time1)) * (center2 - center1);
}

std::optional<Intersection> MovingSphere::intersect(const Ray& ray) const {
    const auto origin = ray.get_origin();
    const auto direction = ray.get_direction();
    const auto center = get_center(ray.get_time());
    const auto v = center - origin;
    const auto u1 = glm::dot(direction, v);
    const auto d_2 = glm::length(v) * glm::length(v) - u1 * u1;
    if (radius * radius >= d_2) {
        const auto u2 = glm::sqrt(radius * radius - d_2);
        const auto t0 = u1 - u2;
        const auto t1 = u1 + u2;
        if (t0 >= 0) {
            return Intersection(t0, origin + direction * t0, glm::normalize(origin + direction * t0 - center), material);
        }
        if (t1 >= 0) {
            return Intersection(t1, origin + direction * t1, glm::normalize(origin + direction * t1 - center), material);
        }
    }
    return std::nullopt;
}
