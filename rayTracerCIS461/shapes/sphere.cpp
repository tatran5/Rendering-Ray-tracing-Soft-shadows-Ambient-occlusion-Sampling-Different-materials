#include "sphere.h"
#include "primitive.h"

Sphere::Sphere() {}

bool Sphere::getIntersection(Ray ray, Intersection *p_intersection) const {
    Ray transformedRay = ray.transform(m_transform.m_worldTransformInv);
    Point3f transformedOri = transformedRay.m_origin;
    Vector3f transformedDir = transformedRay.m_direction;

    vec4 interPoint; //intersection with the shape (which might or might not be unit sphere)
    Vector3f interNormal;
    float t;
    float radius = 0.5f;

    //solve equation at^2 + bt + c = 0 for t
    float a = pow(transformedDir.x, 2.f) + pow(transformedDir.y, 2.f) +
            pow(transformedDir.z, 2.f);
    float b = 2.f * (transformedDir.x * transformedOri.x +
                   transformedDir.y * transformedOri.y +
                   transformedDir.z * transformedOri.z);
    float c = pow(transformedOri.x, 2.f) + pow(transformedOri.y, 2.f)
            + pow(transformedOri.z, 2.f) - pow(radius, 2.f);
    float delta = pow(b, 2.f) - 4.f * a * c;
    if (delta < 0) {
        return false;
    } if (delta == 0) {
        t = -b / (2.f * a);
        if (t < 0) {return false;}
    } else {
        float t1 = (-b + std::sqrt(delta)) / (2.f * a);
        float t2 = (-b - std::sqrt(delta)) / (2.f * a);
        if (t1 < 0 && t2 < 0) {return false;}
        else if (t1 < t2 && t1 >= 0) {t = t1;}
        else if (t2 < t1 && t2 >= 0) {t = t2;}
        else if (t1 < 0 && t2 >= 0) {t = t2;}
        else if (t2 < 0 && t1 >= 0) {t = t1;}
        else { //for debugging
            qDebug() << t1;
            qDebug() << t2;
            t = 1.f;
            qDebug() << "unaccounted cases";
            return false;
        }
    }

    Point3f interPointOnUnit = transformedOri + t * transformedDir; //intersection on unit sphere
    interPoint = m_transform.m_worldTransform * vec4(interPointOnUnit, 1.f);
    interNormal = normalize(m_transform.m_worldTransformInvTranspose * interPointOnUnit);

    p_intersection->m_point = Point3f(interPoint.x, interPoint.y, interPoint.z);
   p_intersection->m_normal = interNormal;
    p_intersection->m_t = t;
    return true;
}
