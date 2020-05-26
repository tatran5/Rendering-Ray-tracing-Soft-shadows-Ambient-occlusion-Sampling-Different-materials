#ifndef LIGHT_H
#define LIGHT_H

#include "globalincludes.h"
#include "intersection.h"
#include "scene.h"

class Scene;

class Light
{
public:
    Transform m_transform;
    Color3f m_hueIntensity;
    Scene* mp_scene;

    virtual ~Light();

    virtual bool canSeeLight(Intersection* p_intersection, float* p_lightFract) = 0;
};

#endif // LIGHT_H
