#include <glm/ext/vector_float3.hpp>

#include "materials/material.hpp"

namespace raytracer::materials {

glm::vec3 Material::emit(float /*u*/, float /*v*/, const glm::vec3& /*p*/) const {
    return {};
}

} // namespace raytracer::materials
