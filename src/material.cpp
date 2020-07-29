#include "material.hpp"

Material::Material(const glm::vec3& d)
    : diffuse_color(d) {
}

glm::vec3 Material::get_diffuse_color() const {
    return diffuse_color;
}
