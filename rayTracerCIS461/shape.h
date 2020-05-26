#ifndef SHAPE_H
#define SHAPE_H

#include "transform.h"
#include "ray.h"
#include "primitive.h"

class Intersection;

class Shape
{
public:
    Transform m_transform;

    Shape();
    Shape(Transform transform);
    virtual ~Shape();

    virtual bool getIntersection(Ray ray, Intersection* p_intersection) const = 0;
};

#endif // SHAPE_H
