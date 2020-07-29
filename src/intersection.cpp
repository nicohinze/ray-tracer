#include <memory>

#include "intersection.hpp"
#include "material.hpp"

Intersection::Intersection(float d, const glm::vec3& p, const glm::vec3& n, const Material* m)
    : distance(d), position(p), normal(n), material(m) {}
    