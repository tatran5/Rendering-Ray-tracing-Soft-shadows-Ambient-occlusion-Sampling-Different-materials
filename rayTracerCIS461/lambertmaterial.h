#ifndef LAMBERTMATERIAL_H
#define LAMBERTMATERIAL_H

#include "globalincludes.h"
#include "material.h"
#include "intersection.h"
#include "ray.h"
#include "light.h"
#include "scene.h"
#include "speculartransmissionmaterial.h"

class Scene;

class LambertMaterial : public Material
{
public:
    LambertMaterial();
    LambertMaterial(Color3f color, Scene* p_scene);

    Color3f getScatteredColor(Intersection* p_intersection, Ray* p_ray, int recurLeft) override;
};

#endif // LAMBERTMATERIAL_H
