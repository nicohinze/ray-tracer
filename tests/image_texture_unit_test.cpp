#define CATCH_CONFIG_MAIN

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <glm/ext/vector_float3.hpp>

#include "materials/image_texture.hpp"
#include "utils/utils.hpp"

namespace raytracer::tests::materials {

namespace {
void require_vec_equal(const glm::vec3& vec1, const glm::vec3& vec2) {
    static constexpr auto MARGIN = 0.00001;
    REQUIRE_THAT(vec1.x, Catch::Matchers::WithinAbs(vec2.x, MARGIN));
    REQUIRE_THAT(vec1.y, Catch::Matchers::WithinAbs(vec2.y, MARGIN));
    REQUIRE_THAT(vec1.z, Catch::Matchers::WithinAbs(vec2.z, MARGIN));
}
} // namespace

TEST_CASE("Mango texture values", "[image texture]") {
    const auto data_dir = utils::data_directory_path();
    const auto texture = raytracer::materials::ImageTexture((data_dir.value() / std::filesystem::path("mango.png")).string()); // NOLINT(bugprone-unchecked-optional-access)
    const glm::vec3 p = {};
    require_vec_equal(texture.value(0, 0, p), glm::vec3{0, 1, 0});
    require_vec_equal(texture.value(1, 0, p), glm::vec3{1, 1, 0});
    require_vec_equal(texture.value(0, 1, p), glm::vec3{0, 0, 0});
    require_vec_equal(texture.value(1, 1, p), glm::vec3{1, 0, 0});

    const auto linear2srgb = 0.215764;
    require_vec_equal(texture.value(0.5, 0, p), glm::vec3{linear2srgb, 1, 0});
    require_vec_equal(texture.value(0.5, 1, p), glm::vec3{linear2srgb, 0, 0});
    require_vec_equal(texture.value(0, 0.5, p), glm::vec3{0, linear2srgb, 0});
    require_vec_equal(texture.value(1, 0.5, p), glm::vec3{1, linear2srgb, 0});
    require_vec_equal(texture.value(0.5, 0.5, p), glm::vec3{linear2srgb, linear2srgb, 0});
}

} // namespace raytracer::tests::materials
