#pragma once

#include <glm/glm.hpp>

class Light {
  private:
    glm::vec3 position;
    float intensity;

  public:
    Light(const glm::vec3& p, float i);
    ~Light() = default;
    glm::vec3 get_position() const;
    float get_intensity() const;
};
