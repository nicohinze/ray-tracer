#pragma once

#include <array>
#include <cstddef>
#include <utility>

#include "glm/fwd.hpp"
#include "utils/utils.hpp"

namespace raytracer::noise {

class Perlin {
  private:
    static const int point_count = 256;
    std::array<glm::vec3, point_count> randvec3;
    std::array<int, point_count> perm_x;
    std::array<int, point_count> perm_y;
    std::array<int, point_count> perm_z;

    template <std::size_t N>
    void permute(std::array<int, N>& arr) {
        for (auto i = N - 1; i > 0; --i) {
            auto j = raytracer::utils::random_int(0, i);
            std::swap(arr[i], arr[j]);
        }
    }

    template <std::size_t N>
    void generate_permutation(std::array<int, N>& arr) {
        for (auto i = 0; i < point_count; ++i) {
            arr[i] = i;
        }
        permute(arr);
    }

    float perlin_interpolation(const std::array<std::array<std::array<glm::vec3, 2>, 2>, 2>& corners, float u, float v, float w) const;

  public:
    Perlin();
    float noise(const glm::vec3& p) const;
    float turbulence(const glm::vec3& p, int depth) const;
};
} // namespace raytracer::noise
