#include "geometry/geometry_object.hpp"
#include "materials/material.hpp"

namespace raytracer::geometry {

GeometryObject::GeometryObject(const materials::Material* m)
    : material(m) {
}

} // namespace raytracer::geometry
