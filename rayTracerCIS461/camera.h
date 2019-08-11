#ifndef CAMERA_H
#define CAMERA_H

#include "globalincludes.h"
#include "ray.h"

using namespace glm;

class Camera
{
public:
    Point3f m_eye;
    Point3f m_ref;
    Vector3f m_worldUp;
    float m_fieldOfView;
    float m_nearClip;
    float m_farClip;
    float m_screenWidth;
    float m_screenHeight;
    mat4 m_viewProjectionMx;
    mat4 m_viewProjectionMxInv;

    Camera(Point3f eye, Point3f ref, Vector3f worldUpVec, float fieldOfView,
           float nearClip, float farClip, float screenWidth, float screenHeight);
    Ray rayCast(Point2f pixelCo); //takes in a pixel coordinate

private:
    mat4 getViewTranslateMx();
    mat4 getViewOrientMx();
    mat4 getProjectionMx();
};

#endif // CAMERA_H
