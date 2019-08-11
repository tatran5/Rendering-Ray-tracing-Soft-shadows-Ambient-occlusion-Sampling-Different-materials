#ifndef SPECULARREFLECTIONMATERIAL_H
#define SPECULARREFLECTIONMATERIAL_H

#include "globalincludes.h"
#include "material.h"
#include "intersection.h"
#include "ray.h"
#include "light.h"

class Material;

class SpecularReflectionMaterial : public Material
{
public:
    int m_maxRecursiveDepth;
    int m_curRecursiveDepth;

    SpecularReflectionMaterial();
    SpecularReflectionMaterial(Color3f color, Scene* p_scene, int maxRecursiveDepth);

    Color3f getScatteredColor(Intersection* p_intersection, Ray* p_ray, int recurLeft) override;
};

#endif // SPECULARREFLECTIONMATERIAL_H
