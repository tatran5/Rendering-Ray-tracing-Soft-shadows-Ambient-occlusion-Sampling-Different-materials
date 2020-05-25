#ifndef BLINNPHONGMATERIAL_H
#define BLINNPHONGMATERIAL_H

#include "globalincludes.h"
#include "material.h"
#include "intersection.h"
#include "ray.h"

class BlinnPhongMaterial : public Material
{
public:
    float m_specularPower;
    Color3f m_specularColor;

    BlinnPhongMaterial();
    BlinnPhongMaterial(Color3f color, Scene* p_scene, float specularPower, Color3f specularColor);

    Color3f getScatteredColor(Intersection *p_intersection, Ray *p_ray, int recurLeft) override;
};

#endif // BLINNPHONGMATERIAL_H
