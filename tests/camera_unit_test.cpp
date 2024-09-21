#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <glm/glm.hpp>

#include "camera.hpp"

void require_vec_equal(const glm::vec3& vec1, const glm::vec3& vec2) {
    static constexpr auto MARGIN = 0.00001;
    REQUIRE_THAT(vec1.x, Catch::Matchers::WithinAbs(vec2.x, MARGIN));
    REQUIRE_THAT(vec1.y, Catch::Matchers::WithinAbs(vec2.y, MARGIN));
    REQUIRE_THAT(vec1.z, Catch::Matchers::WithinAbs(vec2.z, MARGIN));
}

TEST_CASE("Default camera settings", "[camera]") {
    auto origin = glm::vec3(0, 0, 0);
    auto c = Camera();
    auto r = c.get_ray(0, 0);
    auto expected = Ray(origin, glm::vec3(-0.68599, -0.51449, -0.51449)); // NOLINT(readability-magic-numbers)
    require_vec_equal(r.get_origin(), expected.get_origin());
    require_vec_equal(r.get_direction(), expected.get_direction());
    r = c.get_ray(0.5, 0.5);
    expected = Ray(origin, glm::vec3(0, 0, -1));
    require_vec_equal(r.get_origin(), expected.get_origin());
    require_vec_equal(r.get_direction(), expected.get_direction());
}

TEST_CASE("Custom camera settings without defocus blur", "[camera]") {
    auto origin = glm::vec3(0, static_cast<float>(M_PI) / 4.0F, -static_cast<float>(M_PI) / 4.0F); // NOLINT(readability-magic-numbers)
    auto lookto = glm::vec3(0, -1, -1);
    auto vup = glm::vec3(0, 1, 0);
    auto vfov = 90.0F;               // NOLINT(readability-magic-numbers)
    auto aspect_ratio = 4.0F / 3.0F; // NOLINT(readability-magic-numbers)
    auto aperture = 0.0F;
    auto focus_dist = 1.0F;
    auto c = Camera(
        origin,
        origin + lookto,
        vup,
        vfov,
        aspect_ratio,
        aperture,
        focus_dist);
    auto r = c.get_ray(0, 0);
    auto expected = Ray(origin, glm::vec3(-0.68599, -0.7276, 0)); // NOLINT(readability-magic-numbers)
    require_vec_equal(r.get_origin(), expected.get_origin());
    require_vec_equal(r.get_direction(), expected.get_direction());
    r = c.get_ray(0.5, 0.5);
    expected = Ray(origin, glm::vec3(0, -0.7071, -0.7071)); // NOLINT(readability-magic-numbers)
    require_vec_equal(r.get_origin(), expected.get_origin());
    require_vec_equal(r.get_direction(), expected.get_direction());
}
