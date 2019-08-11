#include "material.h"
#include <iostream>
#include "speculartransmissionmaterial.h"
#include <QDebug>
Material::~Material() {}

bool Material::specularTransmissiveCheck(Light *p_light, Intersection *p_intersection, Color3f *p_color)
{
    /* Cases:
     *     1. Blocked by an opaque object -> return black (not handled in this function
     *     2. Blocked by a transmissive object at point p
     *          Check if point p -> light has any objects in between (by intersect)
     *              -  If intersect is of the same shape, redo the function
     *              - If intersect is of a different shape, multiply the current color
     *                 by the base color of the material of the current intersecition
        If does not have anything in between, return light color???*/

    float softShadowFract = 1.f;
    Material* p_curMat = p_intersection->mp_objectHit->mp_material;//current material of the object at intersection
    SpecularTransmissionMaterial* p_specTrans = dynamic_cast<SpecularTransmissionMaterial*>(p_curMat);
    if (p_specTrans == nullptr) {
        //The object must be non-opaque
        *p_color = Color3f(0.f, 0.f, 0.f);
        return false;
    } else {
        // The object must be transmisive
        // nextInter when put into CanSeeLight will have its value modified to the next intersection
        Intersection nextInter = Intersection(p_intersection->m_point, p_intersection->m_normal,
                                              p_intersection->m_t, p_intersection->mp_objectHit);
        if (p_light->canSeeLight(&nextInter, &softShadowFract)) {
            Color3f baseCol = p_intersection->mp_objectHit->mp_material->m_color;
            *p_color *= baseCol;
            return true;
        }
        else {

            if (p_intersection->mp_objectHit != nextInter.mp_objectHit) {
                //intersection happens on a different object

                //update the current color by the base color of the current intersection
                *p_color *= p_intersection->mp_objectHit->mp_material->m_color;
            }

            /* Intersection on the same shape means redo this function.
             * Intersection of a different shape after the if also means we have to redo this function*/
            return specularTransmissiveCheck(p_light, &nextInter, p_color);
        }
    }
}


