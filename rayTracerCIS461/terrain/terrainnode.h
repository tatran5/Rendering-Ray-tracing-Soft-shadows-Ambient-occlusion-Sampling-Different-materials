#ifndef NODE_H
#define NODE_H

#include "globalincludes.h"
#include "ray.h"
#include "intersection.h"

class TerrainNode
{
public:
    constexpr static float mInitLength = 1;
    int mDepth;
    // Should only have three vertices representing polygon at the current depth.
    // Order is important due to normal produced
    QList<Point3f> mVertices;

    TerrainNode();
    TerrainNode(int depth);
    TerrainNode(int depth, QList<Point3f> vertices);
    TerrainNode(const TerrainNode& otherNode);

    bool getIntersection(Ray ray, Intersection* p_isect,
                         int maxDepth) const;
};

#endif // NODE_H
