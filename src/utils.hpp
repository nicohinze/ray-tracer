#pragma once

#include <cstdint>

#include <glm/glm.hpp>

void show_progress(std::uint32_t percent, std::size_t total_length);
void show_render_progress(std::uint32_t percent);
float random_float();
glm::vec3 random_in_unit_disk();