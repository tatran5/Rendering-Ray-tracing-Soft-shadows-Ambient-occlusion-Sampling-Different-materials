#ifndef MATERIAL_H
#define MATERIAL_H

#include "globalincludes.h"
#include "intersection.h"
#include "ray.h"
#include "scene.h"
//#include "specularreflectionmaterial.h"

// represents a general surface reflection/transmission model,
// which will be inherited by several different classes.

class Light;
class SpecularTransmissionMaterial;

class Material
{
public:
    Color3f m_color; //the base color of the material
    Scene* mp_scene;

    virtual ~Material();

    /*Compute the direction(s) from which light should reach a point of intersection.
    Input: an Intersection, the Ray that caused the Intersection
    Output: the multiplication of the light color and Material color.*/
    virtual Color3f getScatteredColor(Intersection* p_intersection, Ray* p_ray, int recurLeft) = 0;

    //returns true if the intersection actually sees the light as well
    bool specularTransmissiveCheck(Light* p_light, Intersection* p_intersection, Color3f* p_color);
};

#endif // MATERIAL_H
