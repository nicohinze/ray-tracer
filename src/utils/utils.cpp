#include <asm-generic/ioctls.h>
#include <cstddef>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <optional>
#include <random>
#include <string>
#include <sys/ioctl.h>
#include <system_error>
#include <unistd.h>
#include <vector>

#include <glm/fwd.hpp>
#include <glm/geometric.hpp>

#include "utils.hpp"

namespace raytracer::utils {

namespace {
std::size_t get_terminal_width() {
    struct winsize ws{};
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    return ws.ws_col;
}
} // namespace

void show_progress(std::size_t percent, std::size_t total_length) {
    // 1 space char + 1 [ char + 5 chars for percentage display + 1 ] char = 8 chars
    const auto bar_length = total_length > 8 ? total_length - 8 : 0;
    const auto progress = static_cast<double>(percent * bar_length) / 100.0;
    if (bar_length > 0) {
        std::cout << " [";
        for (auto i = 0u; i < bar_length; ++i) {
            if (i < progress) {
                std::cout << '#';
            } else {
                std::cout << '-';
            }
        }
        std::cout << ']';
    }
    // 1 space char + 3 digits + 1 % char = 5 chars
    if (total_length >= 5) { // NOLINT(readability-magic-numbers)
        std::cout << ' ' << std::setfill('0') << std::setw(3) << percent << '%';
    }
    std::cout << std::flush << '\r';
}

void show_render_progress(std::size_t percent) {
    static const auto PREFIX_STRING = std::string("Rendering:");
    static const auto TOTAL = get_terminal_width();
    std::cout << PREFIX_STRING;
    show_progress(percent, TOTAL - PREFIX_STRING.length());
}

int random_int(int min, int max) {
    return static_cast<int>(random_float(static_cast<float>(min), static_cast<float>(max + 1)));
}

float random_float() {
    static const thread_local auto SEED = 19640;
    static thread_local auto mt = std::mt19937(SEED); // NOLINT(cert-msc32-c,cert-msc51-cpp)
    static thread_local auto dist = std::uniform_real_distribution<float>(0.0, 1.0);
    return dist(mt);
}

float random_float(float min, float max) {
    return min + (max - min) * random_float();
}

glm::vec3 random_in_unit_sphere() {
    static const thread_local auto SEED = 19640;
    static thread_local auto mt = std::mt19937(SEED); // NOLINT(cert-msc32-c,cert-msc51-cpp)
    static thread_local auto dist = std::uniform_real_distribution<float>(-1.0, 1.0);
    while (true) {
        auto p = glm::vec3(dist(mt), dist(mt), dist(mt));
        if (glm::length(p) <= 1) {
            return p;
        }
    }
}

glm::vec3 random_in_unit_disk() {
    static const thread_local auto SEED = 19640;
    static thread_local auto mt = std::mt19937(SEED); // NOLINT(cert-msc32-c,cert-msc51-cpp)
    static thread_local auto dist = std::uniform_real_distribution<float>(-1.0, 1.0);
    while (true) {
        auto p = glm::vec3(dist(mt), dist(mt), 0);
        if (glm::length(p) <= 1) {
            return p;
        }
    }
}

std::optional<std::filesystem::path> data_directory_path() {
    const auto current_path = std::filesystem::current_path();
    for (const auto& rel_path : std::vector<std::filesystem::path>{
             "data/",
             "../data/",
             "../../data/",
         }) {
        std::error_code ec;
        auto abs_path = std::filesystem::canonical(current_path / rel_path, ec);
        if (ec == std::errc()) {
            return abs_path;
        }
    }
    return std::nullopt;
}

} // namespace raytracer::utils
