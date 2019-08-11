#include "ray.h"

Ray::Ray(Point3f origin, Vector3f direction) : m_origin(origin), m_direction(direction)
{}

Ray Ray::transform(mat4 transMx)
{
    vec4 origin = vec4(m_origin.x, m_origin.y, m_origin.z, 1.f);
    vec4 direction = vec4(m_direction.x, m_direction.y, m_direction.z, 0.f);

    origin = transMx * origin;
    direction =  transMx * direction;
    return Ray(Point3f(origin.x, origin.y, origin.z),
                         Vector3f(direction.x, direction.y, direction.z));
}
