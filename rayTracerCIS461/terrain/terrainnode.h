#ifndef NODE_H
#define NODE_H

#include "globalincludes.h"

class TerrainNode
{
public:
    int mDepth;
    // Three vertices representing polygon at the current depth.
    // Order is important due to normal produced
    Point3f mVertices[3];
    // Distance from the origin of the ray to the closest intersection with the
    // current extent
    float mDist;

    TerrainNode();
    TerrainNode(int depth);
};

#endif // NODE_H
