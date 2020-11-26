#include <memory>

#include "geometryobject.hpp"
#include "material.hpp"

GeometryObject::GeometryObject(const Material* m)
    : material(m) {
}
