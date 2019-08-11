#ifndef SCENE_H
#define SCENE_H

#include <QList>
#include "camera.h"
#include "intersection.h"

class Primitive;

class Scene
{
public:
    std::vector<std::unique_ptr<Primitive>> m_listUPointerPrim;
    Camera* p_cam;

    Scene();
    bool getIntersection(Ray ray, Intersection* p_intersection) const;
};

#endif // SCENE_H
