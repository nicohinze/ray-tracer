#pragma once

#include <cstdint>

#include <glm/glm.hpp>

void show_progress(std::uint32_t percent, std::size_t total_length);
void show_render_progress(std::uint32_t percent);
float random_float();
float random_float(float min, float max);
glm::vec3 random_in_unit_sphere();
glm::vec3 random_in_unit_disk();
