#pragma once
#ifndef AMBIENTOCCLUSION_H
#define AMBIENTOCCLUSION_H
#include "globalincludes.h"
#include "intersection.h"
#include "scene.h"

class AmbientOcclusion
{
public:
    Scene* mp_scene;
    int m_numSample;
    float hemisphereRad;

    AmbientOcclusion(int numSample, Scene* p_scene, float hemRadius);

    Color3f getAOCol(Intersection* p_intersection);

    void getTangentBitangent(Vector3f& v1, Vector3f* v2, Vector3f* v3);
};

#endif // AMBIENTOCCLUSION_H
