#include "primitive.h"

Primitive::Primitive() {}

Primitive::Primitive(QString name) : m_name(name) {}

Primitive::~Primitive() {}

bool Primitive::getIntersection(Ray ray, Intersection* p_intersection) const
{
    Shape* p_shape = mup_shape.get();
    if (p_shape != nullptr && p_shape->getIntersection(ray, p_intersection)) {
        p_intersection->mp_objectHit = this;
        return true;
    }
    return false;
}
