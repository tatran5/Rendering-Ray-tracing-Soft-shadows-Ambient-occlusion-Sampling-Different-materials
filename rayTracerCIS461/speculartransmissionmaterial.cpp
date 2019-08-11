#include "speculartransmissionmaterial.h"

SpecularTransmissionMaterial::SpecularTransmissionMaterial()
    : SpecularTransmissionMaterial(Color3f(0.f, 0.f, 0.f), nullptr, 1.f, 1.f, 1.f)
{}

SpecularTransmissionMaterial::SpecularTransmissionMaterial(Color3f color, Scene *p_scene, int maxRecursiveDepth,
                                                           float indexOfRefractionIn, float indexOfRefractionOut)
    : m_maxRecursiveDepth(maxRecursiveDepth), m_curRecursiveDepth(maxRecursiveDepth),
      m_indexOfRefractionIn(indexOfRefractionIn), m_indexOfRefractionOut(indexOfRefractionOut),
      m_curRatio(indexOfRefractionIn / indexOfRefractionOut)
{
    m_color = color;
    mp_scene = p_scene;
}


Color3f SpecularTransmissionMaterial::getScatteredColor(Intersection* p_intersection, Ray* p_ray, int recurLeft)
{
    Vector3f normal, incidentDir, refractedDir;
    Intersection newIntersection;

    incidentDir = glm::normalize(p_ray->m_direction);
    normal = glm::normalize(p_intersection->m_normal);

    float normalDotIncident = glm::dot(normal, incidentDir);
    /* etai is the ior of the medium the ray is in before
     * entering the second medium */
    float etai = 1, etat = m_indexOfRefractionIn / m_indexOfRefractionOut;
    if (normalDotIncident < 0) {
        //outside the surface and want cos(theta) to be positive
        normalDotIncident *= -1;
    } else {
        //inside the surface, cos(theta) is already positive but reverse normal direction
        normal *= -1;
        std::swap(etai, etat);
    }

    float eta = etai / etat; // = n1 / n2
    Vector3f newDir;

    //critical angle check
    if (eta * eta * (1 - glm::pow(glm::dot(incidentDir, normal), 2.f)) > 1) {
        //reflect instead of refract
        newDir = glm::normalize(glm::reflect(incidentDir, normal));
    } else { //actual refract
        newDir = glm::normalize(glm::refract(incidentDir, normal, eta));
    }

    Ray reflectedRay = Ray(p_intersection->m_point, newDir);
    reflectedRay.m_origin += 0.00008f * newDir;

    bool doesHitSomething = mp_scene->getIntersection(reflectedRay, &newIntersection);
    //if we have not run out of recursive depth and the ray hits something
    if (recurLeft != 0 && doesHitSomething) {
        return newIntersection.mp_objectHit->mp_material->getScatteredColor(&newIntersection, &reflectedRay, recurLeft) *
                m_color;
    }

    //if we have not run out of recursive depth and the ray does not hit anything
    if (recurLeft != 0 && !doesHitSomething) {
        return vec3(0.f, 0.f, 0.f);
    }

    //if we run out of recursive depth
    else {
        return newIntersection.mp_objectHit->mp_material->m_color * m_color;
    }
}
