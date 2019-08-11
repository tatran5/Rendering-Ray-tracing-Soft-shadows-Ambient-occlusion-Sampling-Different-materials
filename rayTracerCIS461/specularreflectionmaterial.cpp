#include "specularreflectionmaterial.h"
#include "lambertmaterial.h"

SpecularReflectionMaterial::SpecularReflectionMaterial() :
    SpecularReflectionMaterial(Color3f(0.f, 0.f, 0.f), nullptr, 0){}

SpecularReflectionMaterial::SpecularReflectionMaterial(Color3f color, Scene *p_scene, int maxRecursiveDepth)
{
    m_color = color;
    mp_scene = p_scene;
}

/* should reflect the input Ray about the surface normal and
 * evaluate GetScatteredColor at the point where the reflected Ray intersects the scene.*/
Color3f SpecularReflectionMaterial::getScatteredColor(Intersection *p_intersection, Ray *p_ray, int recurLeft)
{
    Vector3f normal, incidentDir, reflectedDir;
    Intersection newIntersection;

    incidentDir = glm::normalize(p_ray->m_direction);
    normal = glm::normalize(p_intersection->m_normal);
    reflectedDir = glm::normalize(glm::reflect(incidentDir, normal));

    Ray reflectedRay = Ray(p_intersection->m_point, reflectedDir);
    reflectedRay.m_origin += 0.00008f * reflectedDir;


    bool doesHitSomething = mp_scene->getIntersection(reflectedRay, &newIntersection);
    //if we have not run out of recursive depth and the ray hits something
    if (recurLeft != 0 && doesHitSomething) {
        recurLeft --;
        return m_color *
                newIntersection.mp_objectHit->mp_material->getScatteredColor(&newIntersection, &reflectedRay, recurLeft);
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
