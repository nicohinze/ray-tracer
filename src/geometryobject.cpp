#include <memory>

#include "geometryobject.hpp"
#include "material.hpp"

GeometryObject::GeometryObject(const Material* m)
    : material(m) {
}

const Material* GeometryObject::get_material() const {
    return material;
}
