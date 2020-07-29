#include "camera.hpp"

Camera::Camera()
    : origin(glm::vec3(0, 0, 0)) {
}

glm::vec3 Camera::get_origin() const {
    return origin;
}
