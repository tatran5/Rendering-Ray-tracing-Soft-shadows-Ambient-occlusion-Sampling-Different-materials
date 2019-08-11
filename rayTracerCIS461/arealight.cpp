#include "arealight.h"
#include "disc.h"
#include "squareplane.h"
#include "sampler.h"
#include "enums.h"
#include <QDebug>

AreaLight::AreaLight(Transform transform, Color3f hueIntensity, Scene* p_scene,
                     Shape* p_shape, int numSample)
    : mp_shape(p_shape), m_numSample(numSample) {
    m_transform = transform;
    m_hueIntensity = hueIntensity;
    mp_scene = p_scene;
}

bool AreaLight::canSeeLight(Intersection* p_intersection, float *p_lightFract) {

    *p_lightFract = 0.f;

    //get all of the samples
    Sampler sampler = Sampler();
    if (dynamic_cast<Disc*>(mp_shape) != nullptr) {
        sampler.generateSamples(m_numSample, STRATIFIED, DISK_CONCENTRIC);
    } else if (dynamic_cast<SquarePlane*>(mp_shape) != nullptr) {
        //should stays the same
        sampler.generateSamples(m_numSample, STRATIFIED, NONE);
    }
    std::vector<float> samplerPosVec = sampler.peekSamples();
    for (int i = 0; i < samplerPosVec.size(); i += 3) {
        std::vector<float> point = sampler.peekSamples();
        //WARNING might need to flip y and z
        glm::vec4 sampLightPosUntransformed =
                glm::vec4(point[i], point[i + 1], 0, 1.f)
                +  glm::vec4(-0.5, -0.5, 0.f, 0.f);

        //cast a ray from intersection to light
        Point3f interPoint = p_intersection->m_point;
        glm::vec4 sampLightPosVec4 = m_transform.m_worldTransform *
                sampLightPosUntransformed;
        Point3f sampLightPos = Point3f(sampLightPosVec4);
        Ray rayInterToLight = Ray(interPoint, glm::normalize(sampLightPos- interPoint));
        rayInterToLight.m_origin += 0.001f * rayInterToLight.m_direction;

        //check if this ray does not hit anything
        // orhits its closest object in the scene that is the light
        Intersection interWithLight;
        bool doesHitObject = mp_scene->getIntersection(rayInterToLight, &interWithLight);
        if (!doesHitObject || (doesHitObject &&
                               interWithLight.mp_objectHit->mup_light.get() == this) ) {
            *p_lightFract += 1;
        }
    }


    *p_lightFract /= (float) sampler.getNumSamples();
    return (*p_lightFract > 0.f);
}
