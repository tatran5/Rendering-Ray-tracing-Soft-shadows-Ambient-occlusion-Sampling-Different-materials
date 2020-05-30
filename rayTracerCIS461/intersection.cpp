#include "intersection.h"
#include "primitive.h"

Intersection::Intersection() :
    Intersection(Point3f(-INFINITY,-INFINITY, -INFINITY), Normal3f(0, 1, 0),
    -INFINITY, nullptr)
{}

Intersection::Intersection(Point3f point, Normal3f normal, float t, const Primitive *p_objectHit) :
    m_point(point), m_normal(normal), m_t(t), mp_objectHit(p_objectHit) {}
