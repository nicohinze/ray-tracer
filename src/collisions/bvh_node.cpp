#include <algorithm>
#include <atomic>
#include <cstddef>
#include <memory>
#include <optional>
#include <vector>

#include "collisions/aabb.hpp"
#include "collisions/bvh_node.hpp"
#include "collisions/hittable.hpp"
#include "collisions/intersection.hpp"
#include "collisions/ray.hpp"
#include "geometry/geometry_object.hpp"
#include "utils/utils.hpp"

namespace raytracer::collisions {

bool axis_compare(const std::shared_ptr<Hittable>& obj1, const std::shared_ptr<Hittable>& obj2, int axis) {
    auto bb1 = obj1->bounding_box(0, 0);
    auto bb2 = obj2->bounding_box(0, 0);
    return bb1.get_min()[axis] < bb2.get_min()[axis];
}

bool x_axis_compare(const std::shared_ptr<Hittable>& obj1, const std::shared_ptr<Hittable>& obj2) {
    return axis_compare(obj1, obj2, 0);
}

bool y_axis_compare(const std::shared_ptr<Hittable>& obj1, const std::shared_ptr<Hittable>& obj2) {
    return axis_compare(obj1, obj2, 1);
}

bool z_axis_compare(const std::shared_ptr<Hittable>& obj1, const std::shared_ptr<Hittable>& obj2) {
    return axis_compare(obj1, obj2, 2);
}

std::atomic<std::size_t> BVHNode::intersection_tests = 0; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

BVHNode::BVHNode(const std::vector<std::shared_ptr<Hittable>>& objects, float t0, float t1)
    : BVHNode(objects, 0, objects.size(), t0, t1) {
}

BVHNode::BVHNode(const std::vector<std::shared_ptr<Hittable>>& objects, std::size_t start, std::size_t end, float t0, float t1) {
    auto copy = objects;
    const auto axis = utils::random_int(0, 2);
    const auto comparator = (axis == 0) ? x_axis_compare : ((axis == 1) ? y_axis_compare : z_axis_compare);
    const auto size = end - start;
    if (size == 1) {
        left = objects[start];
        right = objects[start];
    } else if (size == 2) {
        if (comparator(objects[start], objects[start + 1])) {
            left = objects[start];
            right = objects[start + 1];
        } else {
            right = objects[start];
            left = objects[start + 1];
        }
    } else {
        std::sort(copy.begin() + static_cast<int>(start), copy.begin() + static_cast<int>(end), comparator);
        const auto mid = start + size / 2;
        left = std::make_shared<BVHNode>(copy, start, mid, t0, t1);
        right = std::make_shared<BVHNode>(copy, mid, end, t0, t1);
    }
    const auto left_bb = left->bounding_box(t0, t1);
    const auto right_bb = right->bounding_box(t0, t1);
    aabb = surrounding_box(left_bb, right_bb);
}

std::optional<Intersection> BVHNode::intersect(const Ray& ray) const {
    intersection_tests++;
    if (aabb.intersect(ray)) {
        const auto left_intersect = left->intersect(ray);
        const auto right_intersect = right->intersect(ray);
        intersection_tests += static_cast<int>(dynamic_cast<geometry::GeometryObject*>(left.get()) != nullptr);
        intersection_tests += static_cast<int>(dynamic_cast<geometry::GeometryObject*>(right.get()) != nullptr);
        if (left_intersect.has_value() && right_intersect.has_value()) {
            return left_intersect->get_distance() <= right_intersect->get_distance() ? left_intersect : right_intersect;
        }
        if (left_intersect.has_value()) {
            return left_intersect;
        }
        if (right_intersect.has_value()) {
            return right_intersect;
        }
    }
    return std::nullopt;
}

AABB BVHNode::bounding_box(float /*t0*/, float /*t1*/) const {
    return aabb;
}

std::size_t BVHNode::get_intersection_tests() {
    return intersection_tests;
}

} // namespace raytracer::collisions
