#ifndef NODE_H
#define NODE_H

#include "globalincludes.h"
#include "ray.h"
#include "intersection.h"

class TerrainNode
{
public:
    int mDepth;
    // Three vertices representing polygon at the current depth.
    // Order is important due to normal produced
    Point3f mVertices[3];

    TerrainNode();
    TerrainNode(int depth);
    TerrainNode(int depth, Point3f vertices[3]);
    TerrainNode(const TerrainNode& otherNode);

    getIntersection(Ray ray);
};

#endif // NODE_H
