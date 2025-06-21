#pragma once

#include <array>
#include <cstddef>
#include <utility>

#include <glm/fwd.hpp>

#include "utils/utils.hpp"

namespace raytracer::noise {

class Perlin {
  private:
    static constexpr std::size_t POINT_COUNT = 256;
    std::array<glm::vec3, POINT_COUNT> randvec3;
    std::array<unsigned, POINT_COUNT> perm_x;
    std::array<unsigned, POINT_COUNT> perm_y;
    std::array<unsigned, POINT_COUNT> perm_z;

    template <std::size_t N>
    void permute(std::array<unsigned, N>& arr) {
        for (auto i = N - 1; i > 0; --i) {
            auto j = raytracer::utils::random_int(0, static_cast<int>(i));
            std::swap(arr[i], arr[j]);
        }
    }

    template <std::size_t N>
    void generate_permutation(std::array<unsigned, N>& arr) {
        for (auto i = 0u; i < POINT_COUNT; ++i) {
            arr[i] = i;
        }
        permute(arr);
    }

    static float perlin_interpolation(const std::array<std::array<std::array<glm::vec3, 2>, 2>, 2>& corners, float u, float v, float w);

  public:
    Perlin();
    float noise(const glm::vec3& p) const;
    float turbulence(const glm::vec3& p, int depth) const;
};
} // namespace raytracer::noise
