#pragma once

#include <glm/glm.hpp>

class Material {
  private:
    const glm::vec3 color;
    const float k_ambient;
    const float k_diffuse;
    const float k_specular;
    const float shininess;

  public:
    Material(const glm::vec3& c, float ka, float kd, float ks, float s);
    ~Material() = default;
    glm::vec3 get_color() const;
    float get_k_ambient() const;
    float get_k_diffuse() const;
    float get_k_specular() const;
    float get_shininess() const;
};
