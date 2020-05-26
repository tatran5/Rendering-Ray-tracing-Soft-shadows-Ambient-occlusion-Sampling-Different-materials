#include "pointlight.h"
#include <QDebug>

PointLight::PointLight() :
    PointLight(Transform(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f)),
               Color3f(0.f, 0.f, 0.f), nullptr) {}

PointLight::PointLight(Transform transform, Color3f hueIntensity, Scene* p_scene)
{
    m_transform = transform;
    m_hueIntensity = hueIntensity;
    mp_scene = p_scene;
}

bool PointLight::canSeeLight(Intersection *p_intersection, float* p_lightFract)
{
    //cast a ray from intersection to light
    Point3f interPoint = p_intersection->m_point;
    vec4 lightPosVec4 = m_transform.m_worldTransform * vec4(0.f, 0.f, 0.f, 1.f);
    Point3f lightPos = Point3f(lightPosVec4.x, lightPosVec4.y, lightPosVec4.z);
    Ray rayInterToLight = Ray(interPoint, lightPos - interPoint);
    rayInterToLight.m_origin += 0.00008f * rayInterToLight.m_direction;

    //check if this ray hits any object in the scene that is not this light itself
    if (mp_scene->getIntersection(rayInterToLight, p_intersection) &&
       p_intersection->mp_objectHit->mup_light.get() != this) {
        return false;
    }
    return true;
}
