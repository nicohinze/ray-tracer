#include <array>
#include <cmath>

#include <glm/fwd.hpp>
#include <glm/geometric.hpp>

#include "perlin.hpp"
#include "utils/utils.hpp"

namespace raytracer::noise {

Perlin::Perlin()
    : randvec3(), perm_x(), perm_y(), perm_z() {
    for (auto& v : randvec3) {
        v = glm::normalize(utils::random_in_unit_sphere());
    }
    generate_permutation(perm_x);
    generate_permutation(perm_y);
    generate_permutation(perm_z);
}

float Perlin::noise(const glm::vec3& p) const {
    auto u = p.x - std::floor(p.x);
    auto v = p.y - std::floor(p.y);
    auto w = p.z - std::floor(p.z);
    const auto i = static_cast<int>(std::floor(p.x));
    const auto j = static_cast<int>(std::floor(p.y));
    const auto k = static_cast<int>(std::floor(p.z));
    std::array<std::array<std::array<glm::vec3, 2>, 2>, 2> corners{};
    for (auto di = 0u; di < 2; ++di) {
        for (auto dj = 0u; dj < 2; ++dj) {
            for (auto dk = 0u; dk < 2; ++dk) {
                corners[di][dj][dk] = randvec3
                    [perm_x[(i + di) & 255u] ^
                     perm_y[(j + dj) & 255u] ^
                     perm_z[(k + dk) & 255u]];
            }
        }
    }
    return perlin_interpolation(corners, u, v, w);
}

float Perlin::turbulence(const glm::vec3& p, int depth) const {
    auto accum = 0.0f;
    auto tmp_p = p;
    auto weight = 1.0f;
    for (auto _ = 0; _ < depth; ++_) {
        accum += weight * noise(tmp_p);
        weight *= 0.5;
        tmp_p *= 2;
    }
    return std::fabs(accum);
}

float Perlin::perlin_interpolation(const std::array<std::array<std::array<glm::vec3, 2>, 2>, 2>& corners, float u, float v, float w) {
    const auto uu = u * u * (3 - 2 * u);
    const auto vv = v * v * (3 - 2 * v);
    const auto ww = w * w * (3 - 2 * w);
    auto accum = 0.0f;
    for (auto i = 0u; i < 2; ++i) {
        for (auto j = 0u; j < 2; ++j) {
            for (auto k = 0u; k < 2; ++k) {
                const auto weight_v = glm::vec3(u - static_cast<float>(i), v - static_cast<float>(j), w - static_cast<float>(k));
                accum +=
                    (static_cast<float>(i) * uu + static_cast<float>(1 - i) * (1 - uu)) *
                    (static_cast<float>(j) * vv + static_cast<float>(1 - j) * (1 - vv)) *
                    (static_cast<float>(k) * ww + static_cast<float>(1 - k) * (1 - ww)) *
                    glm::dot(corners[i][j][k], weight_v);
            }
        }
    }
    return accum;
}

} // namespace raytracer::noise
