#ifndef RAY_H
#define RAY_H

#include "globalincludes.h"

using namespace glm;

class Ray
{
public:
    Point3f m_origin;
    Vector3f m_direction;

    Ray(Point3f origin, Vector3f direction);
    Ray transform(mat4 transMx);
};

#endif // RAY_H
