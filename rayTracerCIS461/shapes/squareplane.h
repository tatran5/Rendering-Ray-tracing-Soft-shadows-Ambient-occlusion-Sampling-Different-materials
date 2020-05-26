#ifndef SQUAREPLANE_H
#define SQUAREPLANE_H

#include "shape.h"

class SquarePlane : public Shape
{
public:
    /*Unit square plane is centered at the origin, have side lengths of 1,
    and have its normal aligned with the Z-axis*/
    SquarePlane();

    bool getIntersection(Ray ray, Intersection *p_intersection) const override;
};

#endif // SQUAREPLANE_H
