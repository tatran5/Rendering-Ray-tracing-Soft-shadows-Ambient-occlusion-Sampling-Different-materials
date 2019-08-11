#include "ambientocclusion.h"
#include "sampler.h"
#include "ray.h"
#include "intersection.h"
#include "scene.h"
#include <QDebug>

AmbientOcclusion::AmbientOcclusion(int numSample, Scene *p_scene, float hemRadius)
    : m_numSample(numSample), mp_scene(p_scene), hemisphereRad(hemRadius)
{}

Color3f AmbientOcclusion::getAOCol(Intersection* p_intersection) {
    Color3f returnCol = Color3f(0.f, 0.f, 0.f);

    Sampler sampler = Sampler();
    sampler.generateSamples(m_numSample, STRATIFIED, HEMISPHERE_COSINE);
    std::vector<float> samplerPosVec = sampler.peekSamples();
    //for each sampler
    for (int s = 0; s < samplerPosVec.size(); s += 3) {
        //get direction from sample's position
        std::vector<float> samplerPosVec = sampler.peekSamples();
        Point3f samPos = Point3f(samplerPosVec[s],
                                 samplerPosVec[s + 1],
                samplerPosVec[s + 2]);
        Point3f samDir = glm::normalize(samPos);
        glm::vec4 samDirV4 = glm::vec4(samDir, 1.f);

        //ambient occlusion calculation
        Vector3f normal = p_intersection->m_normal;
        Vector3f tangent, bitangent;
        getTangentBitangent(normal, &tangent, &bitangent);

        //tangent bitangent matrix to align ray
        glm::mat4 alignMx = glm::mat4(glm::vec4(glm::normalize(tangent), 0.f),
                                      glm::vec4(glm::normalize(bitangent), 0.f),
                                      glm::vec4(glm::normalize(normal), 0.f),
                                      glm::vec4(0.f, 0.f, 0.f, 1.f));

        //orient the direction into world space from tangent space
        samDirV4 = alignMx * samDirV4;
        Vector3f rayDir = glm::normalize(Vector3f(samDirV4));
        Ray ray = Ray(p_intersection->m_point, rayDir);
        ray.m_origin += 0.0001f * rayDir;

        Intersection newInter = Intersection();
        if (!mp_scene->getIntersection(ray, &newInter) ||
                glm::length(newInter.m_point - p_intersection->m_point)
                > hemisphereRad) {
            returnCol += Color3f(1.f, 1.f, 1.f);
        }
    }
    returnCol = 1.f / (float) m_numSample * returnCol;
    return returnCol;
}

void AmbientOcclusion::getTangentBitangent(Vector3f& v1, Vector3f* v2, Vector3f* v3)
{
    if (std::abs(v1.x) > std::abs(v1.y)){
        *v2 = Vector3f(-v1.z, 0, v1.x) / std::sqrt(v1.x * v1.x + v1.z * v1.z);
    } else {
        *v2 = Vector3f(0, v1.z, -v1.y) / std::sqrt(v1.y * v1.y + v1.z * v1.z);
    }
    *v3 = glm::cross(v1, *v2);
}
