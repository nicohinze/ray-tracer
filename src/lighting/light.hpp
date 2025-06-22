#pragma once

#include <glm/ext/vector_float3.hpp>

namespace raytracer::lighting {

class Light {
  private:
    glm::vec3 position;
    float intensity;

  public:
    Light(const glm::vec3& p, float i);
    glm::vec3 get_position() const;
    float get_intensity() const;
};

} // namespace raytracer::lighting
