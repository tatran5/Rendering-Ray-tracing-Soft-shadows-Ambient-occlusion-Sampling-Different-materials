#ifndef AREALIGHT_H
#define AREALIGHT_H
#include "light.h"

class AreaLight : public Light
{
public:
    Shape* mp_shape;
    int m_numSample;

    AreaLight(Transform transform, Color3f hueIntensity, Scene* p_scene, Shape* p_shape, int numSample);
    bool canSeeLight(Intersection* p_intersection, float* p_lightFract) override;
};

#endif // AREALIGHT_H
