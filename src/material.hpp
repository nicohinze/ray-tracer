#pragma once

#include <glm/glm.hpp>

class Material {
  public:
    const glm::vec3 color;
    const float k_ambient;
    const float k_diffuse;
    const float k_specular;
    const float shininess;
    const float reflectivity;
    const float refractive_index;
    const float refractivity;

    Material(const glm::vec3& c, float ka, float kd, float ks, float s, float refl, float ri, float refr);
};
