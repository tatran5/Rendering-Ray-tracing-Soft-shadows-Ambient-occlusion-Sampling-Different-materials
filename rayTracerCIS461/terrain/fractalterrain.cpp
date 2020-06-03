#include "fractalterrain.h"
#include <QList>
#include <QDebug>

FractalTerrain::FractalTerrain(int maxDepth) :
    FractalTerrain(maxDepth, TerrainNode(0)){}

FractalTerrain::FractalTerrain(int maxDepth, TerrainNode rootNode)
    : mMaxDepth(maxDepth), mRootNode(rootNode)
{}

bool FractalTerrain::getIntersection(Ray ray, Intersection *p_intersection) {
    QMap<float, TerrainNode> activeNodes;
    // activeNodes[INFINITY] = mRootNode;

    // TODO: Interesect ray with the initial root first. DELETE  after debug?
    // delete deturn after test?
    return handleNodeIntersection(ray, mRootNode, &activeNodes);

    while (activeNodes.size() != 0) {
        // Get the closest node to the ray
        QMap<float, TerrainNode>::const_iterator iter = activeNodes.constBegin();
        float dist = iter.key();
        TerrainNode node = iter.value();
        activeNodes.remove(dist);

        // Create four children nodes
        // TODO: replace this. test purpose only
        float addHeight = node.mInitLength / pow(2.f, float(node.mDepth));
        Vector3f add = Vector3f(0, addHeight, 0);
        Vector3f v01 = middleVector2(node.mVertices[0], node.mVertices[1], add);
        Vector3f v12 = middleVector2(node.mVertices[1], node.mVertices[2], add);
        Vector3f v20 = middleVector2(node.mVertices[2], node.mVertices[0], add);

        // Node x shares a vertex corresponding to the parent's vertices[x]
        // Except for node3, which is the center triangle/node from current node
        float nextDepth = node.mDepth + 1;
        QList<Point3f> node0Verts = {node.mVertices[0], v01, v20};
        QList<Point3f> node1Verts = {node.mVertices[1], v12, v01};
        QList<Point3f> node2Verts = {node.mVertices[2], v20, v12};
        QList<Point3f> node3Verts = {v01, v12, v20};
        TerrainNode node0 = TerrainNode(nextDepth, node0Verts);
        TerrainNode node1 = TerrainNode(nextDepth, node1Verts);
        TerrainNode node2 = TerrainNode(nextDepth, node2Verts);
        TerrainNode node3 = TerrainNode(nextDepth, node3Verts);

        // Intersect the ray with these childrens' extents
        handleNodeIntersection(ray, node0, &activeNodes);
        handleNodeIntersection(ray, node1, &activeNodes);
        handleNodeIntersection(ray, node2, &activeNodes);
        handleNodeIntersection(ray, node3, &activeNodes);
    }
}

bool FractalTerrain::handleNodeIntersection(Ray ray, TerrainNode node,
                                            QMap<float, TerrainNode> *p_activeNodes) {
    Intersection isect;
    if (node.getIntersection(ray, &isect, mMaxDepth)) {
        p_activeNodes->insert(isect.m_t, node);
        if (node.mDepth == mMaxDepth) {
            // Cull from the active node list the nodes shadowed by the
            // closest node

            // Delete all of the nodes further than the current one except
            // for the furthest in this loop
            for (QMap<float, TerrainNode>::iterator iter =
                 p_activeNodes->upperBound(isect.m_t);
                 iter != p_activeNodes->end();)
            {
                iter = p_activeNodes->erase(iter++);
            }
            // Delete the very last node (furthest)
            QMap<float, TerrainNode>::iterator iterLast = p_activeNodes->end();
            if (iterLast.key() != isect.m_t)
                p_activeNodes->remove(p_activeNodes->end().key());
        }
        return true;
    }
}




