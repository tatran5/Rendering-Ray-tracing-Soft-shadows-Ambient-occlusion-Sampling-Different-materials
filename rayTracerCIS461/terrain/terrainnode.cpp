#include "terrainnode.h"
#include <QDebug>

TerrainNode::TerrainNode() : TerrainNode(-1) {
}

TerrainNode::TerrainNode(int depth)
{
    mDepth = depth;
    mVertices[0] = Point3f(0, 0, 0);
    mVertices[1] = Point3f(0, 0, 0);
    mVertices[2] = Point3f(0, 0, 0);
}

TerrainNode::TerrainNode(int depth, Point3f vertices[3]) : mDepth(depth)
{
    mVertices[0] = vertices[0];
    mVertices[1] = vertices[1];
    mVertices[2] = vertices[2];
}

TerrainNode::TerrainNode(const TerrainNode& otherNode) {
    mDepth = otherNode.mDepth;
    mVertices[0] = otherNode.mVertices[0];
    mVertices[1] = otherNode.mVertices[1];
    mVertices[2] = otherNode.mVertices[2];
}

TerrainNode::getIntersection(Ray ray) {
    qDebug() << "TODO";
}
