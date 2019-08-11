#include "disc.h"

Disc::Disc() {}

bool Disc::getIntersection(Ray ray, Intersection *p_intersection) const {
    /*Unit disc is centered at the origin, have radius of 0.5f,
    and have its normal aligned with the Z-axis*/
    Ray transformedRay = ray.transform(m_transform.m_worldTransformInv);
    Point3f transformedOri = transformedRay.m_origin;
    Vector3f transformedDir = transformedRay.m_direction;

    vec3 interPointOnUnit;
    Vector3f interNormal;
    float t;

    Point3f normal = Vector3f(0.f, 0.f, 1.f); // of the unit disc

    t = dot(normal, Point3f(0.5f, 0.5f, 0.f) - transformedOri) /
            dot(normal, transformedDir);
    if (t < 0) {
        return false;
    }

    interPointOnUnit = transformedOri + t * transformedDir;
    if (glm::pow(interPointOnUnit.x, 2.f) + glm::pow(interPointOnUnit.y, 2.f)
            + glm::pow(interPointOnUnit.z, 2.f)
            > glm::pow(0.5f, 2.f)) {
        return false;
    }
    vec4 interPoint = m_transform.m_worldTransform * vec4(interPointOnUnit, 1.f);
    interNormal = normalize(m_transform.m_worldTransformInvTranspose * normal);

    p_intersection->m_point = Point3f(interPoint.x, interPoint.y, interPoint.z);
    p_intersection->m_normal = interNormal;
    p_intersection->m_t = t;
    return true;
}

