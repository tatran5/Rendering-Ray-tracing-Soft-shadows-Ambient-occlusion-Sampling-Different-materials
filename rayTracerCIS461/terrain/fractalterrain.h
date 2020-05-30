#ifndef FRACTALTERRAIN_H
#define FRACTALTERRAIN_H
#include "terrainnode.h"

class FractalTerrain
{
public:
    // Testing purpose only
    TerrainNode mRootNode;

    FractalTerrain();
    FractalTerrain(TerrainNode rootNode);

    getIntersection(Ray ray, Intersection *p_intersection);
};

#endif // FRACTALTERRAIN_H
