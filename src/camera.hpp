#pragma once

#include <glm/glm.hpp>

class Camera {
  private:
    glm::vec3 origin;

  public:
    Camera();
    ~Camera() = default;
    glm::vec3 get_origin() const;
};
