#include "ray.hpp"

Ray::Ray(const glm::vec3& o, const glm::vec3& d)
    : origin(o), direction(d) {}
