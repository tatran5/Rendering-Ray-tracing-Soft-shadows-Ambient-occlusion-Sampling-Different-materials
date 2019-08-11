#ifndef SPHERE_H
#define SPHERE_H

#include <QDebug>
#include "shape.h"

using namespace std;

class Sphere : public Shape
{
public:
    //Unit sphere is assumed to be centered at the origin, radius = 0.5
    Sphere();

    bool getIntersection(Ray ray, Intersection *p_intersection) const override;
};

#endif // SPHERE_H
