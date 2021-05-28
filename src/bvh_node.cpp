#include <algorithm>
#include <deque>
#include <iostream>
#include <vector>

#include "bvh_node.hpp"
#include "geometry_object.hpp"
#include "utils.hpp"

bool axis_compare(const std::shared_ptr<Hittable>& obj1, const std::shared_ptr<Hittable>& obj2, int axis) {
    auto bb1 = obj1->bounding_box(0, 0);
    auto bb2 = obj2->bounding_box(0, 0);
    return bb1->get_min()[axis] < bb2->get_min()[axis];
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

std::atomic<std::uint64_t> BVH_Node::intersection_tests = 0; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

BVH_Node::BVH_Node(const std::vector<std::shared_ptr<Hittable>>& objects, float t0, float t1)
    : BVH_Node(objects, 0, objects.size(), t0, t1) {
}

BVH_Node::BVH_Node(const std::vector<std::shared_ptr<Hittable>>& objects, std::size_t start, std::size_t end, float t0, float t1) {
    auto copy = objects;
    const auto axis = random_int(0, 2);
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
        std::sort(copy.begin() + start, copy.begin() + end, comparator);
        const auto mid = start + size / 2;
        left = std::make_shared<BVH_Node>(copy, start, mid, t0, t1);
        right = std::make_shared<BVH_Node>(copy, mid, end, t0, t1);
    }
    const auto left_bb = left->bounding_box(t0, t1);
    const auto right_bb = right->bounding_box(t0, t1);
    if (left_bb.has_value() && right_bb.has_value()) {
        aabb = surrounding_box(left_bb.value(), right_bb.value());
    } else {
        std::cerr << "No valid BB";
    }
}

std::optional<Intersection> BVH_Node::intersect(const Ray& ray) const {
    intersection_tests++;
    if (const auto intersect = aabb.intersect(ray)) {
        const auto left_intersect = left->intersect(ray);
        const auto right_intersect = right->intersect(ray);
        intersection_tests += static_cast<int>(dynamic_cast<GeometryObject*>(left.get()) != nullptr);
        intersection_tests += static_cast<int>(dynamic_cast<GeometryObject*>(right.get()) != nullptr);
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

std::optional<AABB> BVH_Node::bounding_box(float /*t0*/, float /*t1*/) const {
    return aabb;
}

std::uint64_t BVH_Node::get_intersection_tests() {
    return intersection_tests;
}
