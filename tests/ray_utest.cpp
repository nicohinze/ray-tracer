#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file

#include "catch.hpp"
#include <glm/glm.hpp>

#include "ray.hpp"

TEST_CASE("Ray reflection", "[ray]") {
    auto ray = Ray(glm::vec3(0, 0, 0), glm::vec3(1, 1, 0));
    auto n = glm::vec3(-1, 0, 0);
    auto expected = glm::vec3(-1, 1, 0);
    auto reflect = ray.reflect(n);
    REQUIRE(reflect == expected);

    ray = Ray(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
    n = glm::vec3(-1, 0, 0);
    expected = glm::vec3(0, 0, 0);
    reflect = ray.reflect(n);
    REQUIRE(reflect == expected);

    ray = Ray(glm::vec3(0, 0, 0), glm::vec3(1, 1, 0));
    n = glm::vec3(0, 0, 0);
    expected = glm::vec3(1, 1, 0);
    reflect = ray.reflect(n);
    REQUIRE(reflect == expected);
}

TEST_CASE("Ray refraction", "[ray]") {
    auto ray = Ray(glm::vec3(0, 0, 0), glm::vec3(1, 1, 0));
    auto n = glm::vec3(-1, 0, 0);
    auto expected = glm::vec3(1, 1, 0);
    auto refract = ray.refract(n, 1.0);
    REQUIRE(refract == expected);

    ray = Ray(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
    n = glm::vec3(-1, 0, 0);
    expected = glm::vec3(0, 0, 0);
    refract = ray.refract(n, 1.0);
    REQUIRE(refract == expected);

    ray = Ray(glm::vec3(0, 0, 0), glm::vec3(1, 1, 0));
    n = glm::vec3(-1, 0, 0);
    expected = glm::vec3(1, 2.0 / 3.0, 0);
    refract = ray.refract(n, 1.5);
    REQUIRE(refract == expected);
}
