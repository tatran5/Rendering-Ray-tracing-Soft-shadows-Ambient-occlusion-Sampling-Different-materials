#include "scene.h"
#include <QDebug>
#include "sampler.h"

Scene::Scene() {}

bool Scene::getIntersection(Ray ray, Intersection* p_intersection) const {
    p_intersection->m_t = INFINITY;
    bool isIntersected = false;
    bool tempIsIntersected = false;
    //for each primitive
    for (int i = 0; i < m_listUPointerPrim.size(); i++) {
        Intersection curIntersection = Intersection();
        Primitive* p_curPrimitive = m_listUPointerPrim.at(i).get();
        tempIsIntersected = p_curPrimitive->getIntersection(ray, &curIntersection);


        if (tempIsIntersected && curIntersection.m_t < p_intersection->m_t) {
            isIntersected = true;
            p_intersection->m_t = curIntersection.m_t;
            p_intersection->m_normal = curIntersection.m_normal;
            p_intersection->m_point = curIntersection.m_point;
            p_intersection->mp_objectHit = m_listUPointerPrim.at(i).get();
        }
    }
    return isIntersected;
}
