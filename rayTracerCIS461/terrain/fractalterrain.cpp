#include "fractalterrain.h"
#include <QList>
#include <QDebug>

FractalTerrain::FractalTerrain() : FractalTerrain(TerrainNode(0)){}

FractalTerrain::FractalTerrain(TerrainNode rootNode) : mRootNode(rootNode)
{}

FractalTerrain::getIntersection(Ray ray, Intersection *p_intersection) {
    QMap<float, TerrainNode> activeNodes;
    activeNodes[123] = mRootNode;
    activeNodes[23] = mRootNode;
    activeNodes[134] = mRootNode;
    activeNodes[19] = mRootNode;

    QMapIterator<float, TerrainNode> i(activeNodes);
    while (i.hasNext()) {
        i.next();
        qDebug() << i.key();
    }
}
