#include "material.hpp"

Material::Material(const glm::vec3& c, float ka, float kd, float ks, float s, float r)
    : color(c), k_ambient(ka), k_diffuse(kd), k_specular(ks), shininess(s), reflectivity(r) {
}

glm::vec3 Material::get_color() const {
    return color;
}

float Material::get_k_ambient() const {
    return k_ambient;
}

float Material::get_k_diffuse() const {
    return k_diffuse;
}

float Material::get_k_specular() const {
    return k_specular;
}

float Material::get_shininess() const {
    return shininess;
}

float Material::get_reflectivity() const {
    return reflectivity;
}
