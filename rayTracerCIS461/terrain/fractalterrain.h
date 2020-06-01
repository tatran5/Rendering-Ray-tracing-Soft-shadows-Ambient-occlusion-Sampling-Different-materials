#ifndef FRACTALTERRAIN_H
#define FRACTALTERRAIN_H
#include "terrainnode.h"
#include "globalincludes.h"

class FractalTerrain
{
public:
    // Testing purpose only
    TerrainNode mRootNode;
    int mMaxDepth;

    FractalTerrain(int maxDepth);
    FractalTerrain(int maxDepth, TerrainNode rootNode);

    bool getIntersection(Ray ray, Intersection *p_intersection);
    void handleNodeIntersection(Ray ray, TerrainNode node,
                                QMap<float, TerrainNode> *p_activeNodes);
};

#endif // FRACTALTERRAIN_H
