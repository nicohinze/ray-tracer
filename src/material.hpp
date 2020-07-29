#pragma once

#include <glm/glm.hpp>

class Material {
  private:
    glm::vec3 diffuse_color;

  public:
    Material(const glm::vec3& d);
    ~Material() = default;
    glm::vec3 get_diffuse_color() const;
};
