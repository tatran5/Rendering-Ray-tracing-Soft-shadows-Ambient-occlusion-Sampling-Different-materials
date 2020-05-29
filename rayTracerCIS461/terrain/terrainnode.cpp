#include "terrainnode.h"

TerrainNode::TerrainNode() : TerrainNode(-1) {
}

TerrainNode::TerrainNode(int depth) : mDepth(depth),
    mVertices{Point3f(0, 0, 0), Point3f(0, 0, 0), Point3f(0, 0, 0)},
    mDist(-INFINITY)
{}
