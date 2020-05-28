#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include "light.h"

class PointLight : public Light
{
public:

    PointLight();
    PointLight(Transform transform, Color3f hueIntensity, Scene* p_scene);

    bool canSeeLight(Intersection *p_intersection, float* p_lightFract) override;
};

#endif // POINTLIGHT_H
