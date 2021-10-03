#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include <glm/glm.hpp>

#include "intersection.hpp"
#include "ray.hpp"
#include "sphere.hpp"

static constexpr auto MARGIN = 0.001;

void require_vec_equal(const glm::vec3& vec1, const glm::vec3& vec2) {
    REQUIRE(vec1.x == Approx(vec2.x).margin(MARGIN));
    REQUIRE(vec1.y == Approx(vec2.y).margin(MARGIN));
    REQUIRE(vec1.z == Approx(vec2.z).margin(MARGIN));
}

TEST_CASE("Ray-sphere intersection", "[sphere]") {
    auto sphere = Sphere(glm::vec3(0, 0, 0), 1, nullptr);
    auto ray = Ray(glm::vec3(0, 1, 1), glm::vec3(0, 0, -1));
    auto expected = Intersection(1.0, glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), nullptr, 0.25, 1); // NOLINT(readability-magic-numbers)
    auto intersec = sphere.intersect(ray);
    REQUIRE(intersec->get_distance() == Approx(expected.get_distance()).margin(MARGIN));
    require_vec_equal(intersec->get_position(), expected.get_position());
    require_vec_equal(intersec->get_normal(), expected.get_normal());
    REQUIRE(intersec->get_u() == expected.get_u());
    REQUIRE(intersec->get_v() == expected.get_v());

    sphere = Sphere(glm::vec3(0, 1, 0), 1, nullptr);
    ray = Ray(glm::vec3(0, -1, 0), glm::vec3(0, 1, 0));
    expected = Intersection(1.0, glm::vec3(0, 0, 0), glm::vec3(0, -1, 0), nullptr, 0.5, 0);
    intersec = sphere.intersect(ray);
    REQUIRE(intersec->get_distance() == Approx(expected.get_distance()).margin(MARGIN));
    require_vec_equal(intersec->get_position(), expected.get_position());
    require_vec_equal(intersec->get_normal(), expected.get_normal());
    REQUIRE(intersec->get_u() == expected.get_u());
    REQUIRE(intersec->get_v() == expected.get_v());

    sphere = Sphere(glm::vec3(0, 0, 0), 1, nullptr);
    ray = Ray(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    expected = Intersection(1.0, glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), nullptr, 0.5, 1);
    intersec = sphere.intersect(ray);
    REQUIRE(intersec->get_distance() == Approx(expected.get_distance()).margin(MARGIN));
    require_vec_equal(intersec->get_position(), expected.get_position());
    require_vec_equal(intersec->get_normal(), expected.get_normal());
    REQUIRE(intersec->get_u() == expected.get_u());
    REQUIRE(intersec->get_v() == expected.get_v());

    sphere = Sphere(glm::vec3(0, 0, 0), 1, nullptr);
    ray = Ray(glm::vec3(0, -2, 0), glm::vec3(1, 0, 0));
    intersec = sphere.intersect(ray);
    REQUIRE(!intersec.has_value());
}
