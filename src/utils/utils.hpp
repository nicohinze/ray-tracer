#pragma once

#include <cstddef>
#include <filesystem>
#include <optional>

#include <glm/ext/vector_float3.hpp>

namespace raytracer::utils {

void show_progress(std::size_t percent, std::size_t total_length);
void show_render_progress(std::size_t percent);
int random_int(int min, int max);
float random_float();
float random_float(float min, float max);
glm::vec3 random_in_unit_sphere();
glm::vec3 random_in_unit_disk();
std::optional<std::filesystem::path> data_directory_path();

} // namespace raytracer::utils
