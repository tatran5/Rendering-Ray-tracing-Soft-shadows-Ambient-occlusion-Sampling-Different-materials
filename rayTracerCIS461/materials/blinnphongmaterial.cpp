#include "blinnphongmaterial.h"

BlinnPhongMaterial::BlinnPhongMaterial() : BlinnPhongMaterial(Color3f(0.f, 0.f, 0.f), nullptr,
                                                              70.f, Color3f(0.f, 0.f, 0.f)) {}

BlinnPhongMaterial::BlinnPhongMaterial(Color3f color, Scene* p_scene, float specularPower, Color3f specularColor)
    : m_specularPower(specularPower), m_specularColor(specularColor)
{
    m_color = color;
    mp_scene = p_scene;
}

/* Should sample all light sources in the scene for their color (testing for visibility)
and return the sum of their colors multiplied with the Material base color. */
Color3f BlinnPhongMaterial::getScatteredColor(Intersection *p_intersection, Ray *p_ray, int recurLeft)
{
    Color3f diffuse = Color3f(0.f, 0.f, 0.f);
    Color3f specular = Color3f(0.f, 0.f, 0.f);
    Color3f color = Color3f(0.f, 0.f, 0.f);
    float blendWeight = 0.5;

    //iterate through all of the primitives in the scene to check for light sources
    for (int i = 0; i < mp_scene->m_listUPointerPrim.size(); i ++) {
        Primitive* p_prim = mp_scene->m_listUPointerPrim.at(i).get();
        Light* p_light = p_prim->mup_light.get();
        Intersection interCopy = Intersection(p_intersection->m_point,
                                              p_intersection->m_normal,
                                              p_intersection->m_t,
                                              p_intersection->mp_objectHit);

        /* check if
         * the current primitive has a light source
         * the given intersection can see that light source */

        /* Note: canSeeLight should handle the case when the light is
         * testing the intesection on the light's shape*/
        float softShadowFract = 1.f;
        bool noObjectBetween = false;
        bool noOpaqueBetween = false;
        if (p_light != nullptr) {
            noObjectBetween = p_light->canSeeLight(&interCopy, &softShadowFract);
        }

        if (p_light != nullptr && !noObjectBetween)  {
            //check for conditions if the object blocking is opaque or not
            noOpaqueBetween = specularTransmissiveCheck(p_light, &interCopy, &color);
        }

        if (p_light != nullptr && (noObjectBetween || noOpaqueBetween)) {

            //update the diffuse part
            Color3f hueIntensity = p_light->m_hueIntensity;

            //Apply lamber's cosine law
            //get light position
            vec4 lightPosV4 = p_light->m_transform.m_worldTransform * vec4(0.f, 0.f, 0.f, 1.f);
            Vector3f lightPos = Vector3f(lightPosV4.x, lightPosV4.y, lightPosV4.z);
            Vector3f lightDirNotNormalized = lightPos - p_intersection->m_point;

            //the light is not testing against itself
            Vector3f lightDir = glm::normalize(lightDirNotNormalized);
            Color3f lightAdded = hueIntensity * glm::dot(p_intersection->m_normal, lightDir);

            //need the dot product with the light BEFORE adding
            diffuse += lightAdded;

            //update the specular part
            Vector3f viewDir = glm::normalize(-p_ray->m_direction);
            Vector3f halfwayVec = glm::normalize(viewDir + lightDir);
            float spec = glm::pow(glm::dot(p_intersection->m_normal, halfwayVec), m_specularPower);
            specular += p_light->m_hueIntensity * spec;
            color += diffuse * (1 - blendWeight) + specular * m_specularColor * blendWeight;
        }
    }

    color = color * m_color; //currently only consider diffuse color

    //clamp colors
    if (color.x > 1) { color.x = 1;}
    if (color.y > 1) { color.y = 1;}
    if (color.z > 1) { color.z = 1;}
    if (color.x < 0) { color.x = 0;}
    if (color.y < 0) { color.y = 0;}
    if (color.z < 0) { color.z = 0;}

    return color;
}
