#include "material.hpp"

Material::Material(const glm::vec3& c, float ka, float kd, float ks, float s, float refl, float ri, float refr)
    : color(c), k_ambient(ka), k_diffuse(kd), k_specular(ks), shininess(s), reflectivity(refl), refractive_index(ri), refractivity(refr) {
}
