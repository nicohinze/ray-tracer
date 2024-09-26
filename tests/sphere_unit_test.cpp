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

void require_vec_equal(const glm::vec3& vec1, const glm::vec3& vec2) {
    REQUIRE_THAT(vec1.x, Catch::Matchers::WithinAbs(vec2.x, MARGIN));
    REQUIRE_THAT(vec1.y, Catch::Matchers::WithinAbs(vec2.y, MARGIN));
    REQUIRE_THAT(vec1.z, Catch::Matchers::WithinAbs(vec2.z, MARGIN));
}

TEST_CASE("Ray-sphere intersection", "[sphere]") {
    auto sphere = raytracer::geometry::Sphere(glm::vec3(0, 0, 0), 1, nullptr);
    auto ray = collisions::Ray(glm::vec3(0, 1, 1), glm::vec3(0, 0, -1));
    auto expected = collisions::Intersection(1.0, glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), nullptr, 0.25, 1); // NOLINT(readability-magic-numbers)
    auto intersec = sphere.intersect(ray);
    if (intersec.has_value()) {
        REQUIRE_THAT(intersec->get_distance(), Catch::Matchers::WithinAbs(expected.get_distance(), MARGIN));
        require_vec_equal(intersec->get_position(), expected.get_position());
        require_vec_equal(intersec->get_normal(), expected.get_normal());
        REQUIRE(intersec->get_u() == expected.get_u());
        REQUIRE(intersec->get_v() == expected.get_v());
    } else {
        FAIL("No intersection occured.");
    }

    sphere = raytracer::geometry::Sphere(glm::vec3(0, 1, 0), 1, nullptr);
    ray = collisions::Ray(glm::vec3(0, -1, 0), glm::vec3(0, 1, 0));
    expected = collisions::Intersection(1.0, glm::vec3(0, 0, 0), glm::vec3(0, -1, 0), nullptr, 0.5, 0);
    intersec = sphere.intersect(ray);
    if (intersec.has_value()) {
        REQUIRE_THAT(intersec->get_distance(), Catch::Matchers::WithinAbs(expected.get_distance(), MARGIN));
        require_vec_equal(intersec->get_position(), expected.get_position());
        require_vec_equal(intersec->get_normal(), expected.get_normal());
        REQUIRE(intersec->get_u() == expected.get_u());
        REQUIRE(intersec->get_v() == expected.get_v());
    } else {
        FAIL("No intersection occured.");
    }

    sphere = raytracer::geometry::Sphere(glm::vec3(0, 0, 0), 1, nullptr);
    ray = collisions::Ray(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    expected = collisions::Intersection(1.0, glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), nullptr, 0.5, 1);
    intersec = sphere.intersect(ray);
    if (intersec.has_value()) {
        REQUIRE_THAT(intersec->get_distance(), Catch::Matchers::WithinAbs(expected.get_distance(), MARGIN));
        require_vec_equal(intersec->get_position(), expected.get_position());
        require_vec_equal(intersec->get_normal(), expected.get_normal());
        REQUIRE(intersec->get_u() == expected.get_u());
        REQUIRE(intersec->get_v() == expected.get_v());
    } else {
        FAIL("No intersection occured.");
    }

    sphere = raytracer::geometry::Sphere(glm::vec3(0, 0, 0), 1, nullptr);
    ray = collisions::Ray(glm::vec3(0, -2, 0), glm::vec3(1, 0, 0));
    intersec = sphere.intersect(ray);
    REQUIRE(!intersec.has_value());
}

} // namespace raytracer::tests::geometry
