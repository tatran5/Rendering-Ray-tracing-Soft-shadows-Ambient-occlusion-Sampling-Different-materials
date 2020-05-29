#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <QString>
#include <memory>
#include "intersection.h"
#include "ray.h"
#include "shape.h"
#include "light.h"
#include "material.h"

using namespace std;

class Intersection;
class Shape;
class Light;
class Material;

class Primitive
{
public:
    QString m_name;
    unique_ptr<Shape> mup_shape;
    unique_ptr<Light> mup_light;
    Material* mp_material;

    Primitive();
    Primitive(QString name);

    ~Primitive();

    bool getIntersection(Ray ray, Intersection* p_intersection) const;
};

#endif // PRIMITIVE_H
