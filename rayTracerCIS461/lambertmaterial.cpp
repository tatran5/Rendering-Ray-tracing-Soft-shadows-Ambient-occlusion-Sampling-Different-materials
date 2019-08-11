#include "lambertmaterial.h"
#include <qdebug.h>

LambertMaterial::LambertMaterial() : LambertMaterial(Color3f(0.f, 0.f, 0.f), nullptr) {}

LambertMaterial::LambertMaterial(Color3f color, Scene* p_scene)
{
    m_color = color;
    mp_scene = p_scene;
}

/* Should sample all light sources in the scene for their color (testing for visibility)
and return the sum of their colors multiplied with the Material base color. */
Color3f LambertMaterial::getScatteredColor(Intersection *p_intersection, Ray *p_ray, int recurLeft)
{
    Color3f color = Color3f(0.f, 0.f, 0.f);
    float numLight = 0.f;


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
        float softShadowFract = 1.f;
        bool noObjectBetween = false;
        bool noOpaqueBetween = false;
        Color3f colorUpdated = Color3f(0.f, 0.f, 0.f);

        if (p_light != nullptr) {
            numLight ++;
            noObjectBetween = p_light->canSeeLight(&interCopy, &softShadowFract);
        }

        if (p_light != nullptr && !noObjectBetween)  {
            //check for conditions if the object blocking is opaque or not

            //in case the color starts out as 0, making the update also 0
            colorUpdated = color;
            if (color.x == 0 && color.y == 0 && color.z == 0) {
                colorUpdated = Color3f(1.f, 1.f, 1.f);
            }

            noOpaqueBetween = specularTransmissiveCheck(p_light, &interCopy, &colorUpdated);

            if (!noOpaqueBetween) {
                colorUpdated = Color3f(0.f, 0.f, 0.f);
            }
        }

        /* Note: canSeeLight should handle the case when the light is
                * testing the intesection on the light's shape*/
        if (p_light != nullptr && (noObjectBetween || noOpaqueBetween)) {

            //no objects in between the light source and this intersection
            Color3f hueIntensity = p_light->m_hueIntensity;
            //Apply lamber's cosine law
            //get light position
            vec4 lightPosV4 = p_light->m_transform.m_worldTransform * vec4(0.f, 0.f, 0.f, 1.f);
            Vector3f lightPos = Vector3f(lightPosV4.x, lightPosV4.y, lightPosV4.z);
            Vector3f lightDirNotNormalized = lightPos - p_intersection->m_point;

            //the light is not testing against itself
            Vector3f lightDir = glm::normalize(lightDirNotNormalized);
            Color3f lightAdded = hueIntensity * glm::dot(p_intersection->m_normal,
                                                         lightDir);
            color += lightAdded /*+ colorUpdated*/;
            if (colorUpdated.x != 0.f || colorUpdated.y != 0.f || colorUpdated.z != 0.f) {
                color *= colorUpdated;
            }
        }

        color *= softShadowFract;
    }

    color = color * m_color / numLight;

    //clamp colors
    if (color.x > 1) { color.x = 1;}
    if (color.y > 1) { color.y = 1;}
    if (color.z > 1) { color.z = 1;}
    if (color.x < 0) { color.x = 0;}
    if (color.y < 0) { color.y = 0;}
    if (color.z < 0) { color.z = 0;}

    return color;
}
