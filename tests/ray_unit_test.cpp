#define CATCH_CONFIG_MAIN

#include <catch2/catch_test_macros.hpp>

#include "collisions/ray.hpp"

namespace raytracer::tests::collisions {

TEST_CASE("Ray reflection", "[ray]") {
    auto ray = raytracer::collisions::Ray(glm::vec3(0, 0, 0), glm::vec3(1, 1, 0));
    auto n = glm::vec3(-1, 0, 0);
    auto expected = glm::vec3(-1, 1, 0);
    auto reflect = ray.reflect(n);
    REQUIRE(reflect == expected);

    ray = raytracer::collisions::Ray(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
    n = glm::vec3(-1, 0, 0);
    expected = glm::vec3(0, 0, 0);
    reflect = ray.reflect(n);
    REQUIRE(reflect == expected);

    ray = raytracer::collisions::Ray(glm::vec3(0, 0, 0), glm::vec3(1, 1, 0));
    n = glm::vec3(0, 0, 0);
    expected = glm::vec3(1, 1, 0);
    reflect = ray.reflect(n);
    REQUIRE(reflect == expected);
}

TEST_CASE("Ray refraction", "[ray]") {
    auto ray = raytracer::collisions::Ray(glm::vec3(0, 0, 0), glm::vec3(1, 1, 0));
    auto n = glm::vec3(-1, 0, 0);
    auto expected = glm::vec3(1, 1, 0);
    auto refract = ray.refract(n, 1.0);
    REQUIRE(refract == expected);

    ray = raytracer::collisions::Ray(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
    n = glm::vec3(-1, 0, 0);
    expected = glm::vec3(0, 0, 0);
    refract = ray.refract(n, 1.0);
    REQUIRE(refract == expected);

    ray = raytracer::collisions::Ray(glm::vec3(0, 0, 0), glm::vec3(1, 1, 0));
    n = glm::vec3(-1, 0, 0);
    expected = glm::vec3(1, 2.0 / 3.0, 0);
    refract = ray.refract(n, 1.5);
    REQUIRE(refract == expected);

    ray = raytracer::collisions::Ray(glm::vec3(0, 0, 0), glm::vec3(1, 1, 0));
    n = glm::vec3(1, 0, 0);
    expected = glm::vec3(1, 1.5, 0);
    refract = ray.refract(n, 1.5);
    REQUIRE(refract == expected);

    ray = raytracer::collisions::Ray(glm::vec3(0, 0, 0), glm::vec3(1, 0.1, 0));
    n = glm::vec3(0, 1, 0);
    expected = glm::vec3(1, -0.1, 0);
    refract = ray.refract(n, 1.5);
    REQUIRE(refract == expected);
}

} // namespace raytracer::tests::collisions
