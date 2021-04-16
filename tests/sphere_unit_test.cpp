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
    auto expected = Intersection(1.0, glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), nullptr);
    auto intersec = sphere.intersect(ray);
    REQUIRE(intersec.value().get_distance() == Approx(expected.get_distance()).margin(MARGIN));
    require_vec_equal(intersec.value().get_position(), expected.get_position());
    require_vec_equal(intersec.value().get_normal(), expected.get_normal());

    sphere = Sphere(glm::vec3(0, 1, 0), 1, nullptr);
    ray = Ray(glm::vec3(0, -1, 0), glm::vec3(0, 1, 0));
    expected = Intersection(1.0, glm::vec3(0, 0, 0), glm::vec3(0, -1, 0), nullptr);
    intersec = sphere.intersect(ray);
    REQUIRE(intersec.value().get_distance() == Approx(expected.get_distance()).margin(MARGIN));
    require_vec_equal(intersec.value().get_position(), expected.get_position());
    require_vec_equal(intersec.value().get_normal(), expected.get_normal());

    sphere = Sphere(glm::vec3(0, 0, 0), 1, nullptr);
    ray = Ray(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    expected = Intersection(1.0, glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), nullptr);
    intersec = sphere.intersect(ray);
    REQUIRE(intersec.value().get_distance() == Approx(expected.get_distance()).margin(MARGIN));
    require_vec_equal(intersec.value().get_position(), expected.get_position());
    require_vec_equal(intersec.value().get_normal(), expected.get_normal());

    sphere = Sphere(glm::vec3(0, 0, 0), 1, nullptr);
    ray = Ray(glm::vec3(0, -2, 0), glm::vec3(1, 0, 0));
    intersec = sphere.intersect(ray);
    REQUIRE(!intersec.has_value());
}

TEST_CASE("Sphere-sphere intersection", "[sphere]") {
    auto sphere1 = Sphere(glm::vec3(0, 1, 0), 1, nullptr);
    auto sphere2 = Sphere(glm::vec3(0, 0, 0), 1, nullptr);
    auto sphere3 = Sphere(glm::vec3(0, -1, 0), 1, nullptr);

    REQUIRE(sphere1.intersect(sphere1));
    REQUIRE(sphere1.intersect(sphere2));
    REQUIRE(!sphere1.intersect(sphere3));
    REQUIRE(sphere2.intersect(sphere3));
}
