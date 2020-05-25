#ifndef SPECULARTRANSMISSIONMATERIAL_H
#define SPECULARTRANSMISSIONMATERIAL_H

#include "material.h"
#include "lambertmaterial.h"

class SpecularTransmissionMaterial : public Material
{
public:
    int m_maxRecursiveDepth;
    int m_curRecursiveDepth;
    float m_indexOfRefractionIn;
    float m_indexOfRefractionOut;
    float m_curRatio;

    SpecularTransmissionMaterial();
    SpecularTransmissionMaterial(Color3f color, Scene* p_scene, int maxRecursiveDepth,
                                 float indexOfRefractionIn, float indexOfRefractionOut);

    Color3f getScatteredColor(Intersection* p_intersection, Ray* p_ray, int recurLeft) override;
};

#endif // SPECULARTRANSMISSIONMATERIAL_H
