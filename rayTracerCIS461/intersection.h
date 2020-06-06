#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "globalincludes.h"
#include "primitive.h"

/* This class will represent the set of information relevant
 * to a point of intersection between a Ray and a Primitive
 * in the scene. */

class Primitive;

class Intersection
{
public:
    Point3f m_point; //The point on the surface (in world space) at which the intersection occurred.
    Normal3f m_normal; //The surface normal at the point of intersection in world space.
    float m_t; //The distance along the Ray at which this point of intersection lies.
    const Primitive* mp_objectHit; //A pointer to the Primitive whose surface we are intersecting

    Intersection();
    Intersection(Point3f point, Normal3f normal, float t, const Primitive* p_objectHit);
};

#endif // INTERSECTION_H
