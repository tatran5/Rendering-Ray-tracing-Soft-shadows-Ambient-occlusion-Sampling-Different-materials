#include "terrainnode.h"
#include <QDebug>
#include <cmath>

TerrainNode::TerrainNode() : TerrainNode(-1) {
}

TerrainNode::TerrainNode(int depth)
{
    mDepth = depth;
    mVertices = {Point3f(0, 0, 0), Point3f(0, 0, 0), Point3f(0, 0, 0)};
}

TerrainNode::TerrainNode(int depth, QList<Point3f> vertices) : mDepth(depth)
{
    mVertices = {vertices[0], vertices[1], vertices[2]};
}

TerrainNode::TerrainNode(const TerrainNode& otherNode) :
    mDepth(otherNode.mDepth)
{
    mVertices = {otherNode.mVertices[0], otherNode.mVertices[1],
                 otherNode.mVertices[2]};
}

bool TerrainNode::getIntersection(Ray ray,
                                  Intersection *p_isect,
                                  int maxDepth) const {
    float epsilon = 0.001f;
    if (mDepth < maxDepth) {
        // This node does not have a primtiive facet
        // TODO: CHANGE TO THE RIGHT CALCULATION FOR EXTENT

    } else {
        // This node has a primitive facet. Hence perform ray intersection triangle
        Point3f v0 = mVertices[0];
        Point3f v1 = mVertices[1];
        Point3f v2 = mVertices[2];

        // Equation of plane is n * x = d. The intersection point is x. n is normal of plane
        Point3f n = cross(v1 - v0, v2 - v0);
        if (dot(n, ray.m_direction) == 0) {
            // Plane is parallel to the ray
            p_isect = nullptr;
            return false;
        }
        float d = dot(n, v0);
        float t = (d - dot(n, ray.m_origin)) / dot(n, ray.m_direction);
        // intersection betwen ray and plane
        Point3f p = ray.m_origin + t * ray.m_direction;

        // TODO: DEBUG WHEN DONE
        // Use barycentric coordinates to see if the point lies within the triangle
        float areaP01 = length(cross(v0 - p, v1 - p)) * 0.5f;
        float areaP02 = length(cross(v0 - p, v2 - p)) * 0.5f;
        float areaP12 = length(cross(v1 - p, v2 - p)) * 0.5f;
        float area012 = length(cross(v1 - v0, v2 - v0)) * 0.5f;
        float sumSmallAreas = areaP01 + areaP02 + areaP12;
        if (isnan(sumSmallAreas) || sumSmallAreas < area012 - epsilon || sumSmallAreas > area012 + epsilon) {
            p_isect = nullptr;
            return false;
        }

        *p_isect = Intersection(p, n, t, nullptr);
        return true;
    }
}
