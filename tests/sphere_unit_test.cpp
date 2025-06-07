#include <optional>
#define CATCH_CONFIG_MAIN

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <glm/fwd.hpp>

#include "collisions/intersection.hpp"
#include "collisions/ray.hpp"
#include "geometry/sphere.hpp"

namespace raytracer::tests::geometry {

static constexpr auto MARGIN = 0.001;

namespace {
void require_vec_equal(const glm::vec3& vec1, const glm::vec3& vec2) {
    REQUIRE_THAT(vec1.x, Catch::Matchers::WithinAbs(vec2.x, MARGIN));
    REQUIRE_THAT(vec1.y, Catch::Matchers::WithinAbs(vec2.y, MARGIN));
    REQUIRE_THAT(vec1.z, Catch::Matchers::WithinAbs(vec2.z, MARGIN));
}
} // namespace

TEST_CASE("Ray-sphere intersection", "[sphere]") {
    // NOLINTBEGIN(bugprone-unchecked-optional-access)
    auto sphere1 = raytracer::geometry::Sphere(glm::vec3(0, 0, 0), 1, nullptr);
    auto ray1 = collisions::Ray(glm::vec3(0, 1, 1), glm::vec3(0, 0, -1));
    auto expected1 = collisions::Intersection(1.0, glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), nullptr, 0.25, 1);
    auto intersec1 = sphere1.intersect(ray1);
    REQUIRE(intersec1.has_value());
    REQUIRE_THAT(intersec1->get_distance(), Catch::Matchers::WithinAbs(expected1.get_distance(), MARGIN));
    require_vec_equal(intersec1->get_position(), expected1.get_position());
    require_vec_equal(intersec1->get_normal(), expected1.get_normal());
    REQUIRE(intersec1->get_u() == expected1.get_u());
    REQUIRE(intersec1->get_v() == expected1.get_v());

    auto sphere2 = raytracer::geometry::Sphere(glm::vec3(0, 1, 0), 1, nullptr);
    auto ray2 = collisions::Ray(glm::vec3(0, -1, 0), glm::vec3(0, 1, 0));
    auto expected2 = collisions::Intersection(1.0, glm::vec3(0, 0, 0), glm::vec3(0, -1, 0), nullptr, 0.5, 0);
    auto intersec2 = sphere2.intersect(ray2);
    REQUIRE(intersec2.has_value());
    REQUIRE_THAT(intersec2->get_distance(), Catch::Matchers::WithinAbs(expected2.get_distance(), MARGIN));
    require_vec_equal(intersec2->get_position(), expected2.get_position());
    require_vec_equal(intersec2->get_normal(), expected2.get_normal());
    REQUIRE(intersec2->get_u() == expected2.get_u());
    REQUIRE(intersec2->get_v() == expected2.get_v());

    auto sphere3 = raytracer::geometry::Sphere(glm::vec3(0, 0, 0), 1, nullptr);
    auto ray3 = collisions::Ray(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    auto expected3 = collisions::Intersection(1.0, glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), nullptr, 0.5, 1);
    auto intersec3 = sphere3.intersect(ray3);
    REQUIRE(intersec3.has_value());
    REQUIRE_THAT(intersec3->get_distance(), Catch::Matchers::WithinAbs(expected3.get_distance(), MARGIN));
    require_vec_equal(intersec3->get_position(), expected3.get_position());
    require_vec_equal(intersec3->get_normal(), expected3.get_normal());
    REQUIRE(intersec3->get_u() == expected3.get_u());
    REQUIRE(intersec3->get_v() == expected3.get_v());

    auto sphere4 = raytracer::geometry::Sphere(glm::vec3(0, 0, 0), 1, nullptr);
    auto ray4 = collisions::Ray(glm::vec3(0, -2, 0), glm::vec3(1, 0, 0));
    auto intersec4 = sphere4.intersect(ray4);
    REQUIRE(!intersec4.has_value());
    // NOLINTEND(bugprone-unchecked-optional-access)
}

} // namespace raytracer::tests::geometry
