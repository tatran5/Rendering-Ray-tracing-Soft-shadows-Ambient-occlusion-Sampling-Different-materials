#ifndef DISC_H
#define DISC_H
#include "shape.h"

class Disc : public Shape
{
public:
    Disc();

    bool getIntersection(Ray ray, Intersection *p_intersection) const override;
};

#endif // DISC_H
