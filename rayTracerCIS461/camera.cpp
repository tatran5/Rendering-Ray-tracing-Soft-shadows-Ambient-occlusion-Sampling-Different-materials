#include "camera.h"
#include <math.h>

Camera::Camera(Point3f eye, Point3f ref, Vector3f worldUpVec, float fieldOfView,
               float nearClip, float farClip, float screenWidth, float screenHeight) :
    m_eye(eye), m_ref(ref), m_worldUp(worldUpVec), m_fieldOfView(fieldOfView),
    m_nearClip(nearClip), m_farClip(farClip), m_screenWidth(screenWidth), m_screenHeight(screenHeight)
{
    mat4 viewTranslateMx = getViewTranslateMx();
    mat4 viewOrientMx = getViewOrientMx();
    mat4 projectionMx = getProjectionMx();

    m_viewProjectionMx = projectionMx * viewOrientMx * viewTranslateMx;
    m_viewProjectionMxInv = inverse(m_viewProjectionMx);
}

mat4 Camera::getViewTranslateMx()
{
    return mat4(vec4(1.f, 0.f, 0.f, 0.f),
                vec4(0.f, 1.f, 0.f, 0.f),
                vec4(0.f, 0.f, 1.f, 0.f),
                vec4(- m_eye.x, - m_eye.y, - m_eye.z, 1.f));
}

mat4 Camera::getViewOrientMx()
{
    Vector3f forward, up, right; //of the camera
    forward = normalize(m_ref - m_eye);
    if (forward.x != m_worldUp.x || forward.y != m_worldUp.y || forward.z != m_worldUp.z) {
        right  = normalize(cross(forward, m_worldUp));
        up = normalize(cross(right, forward));
    } else {
        right = Vector3f(1, 0, 0);
        up = Vector3f(0, 0, -1);
    }
    return mat4(vec4(right.x, up.x, forward.x, 0.f),
                vec4(right.y, up.y, forward.y, 0.f),
                vec4(right.z, up.z, forward.z, 0.f),
                vec4(0.f, 0.f, 0.f, 1.f));
}

mat4 Camera::getProjectionMx()
{
    float fovRad = m_fieldOfView * M_PI / 180.f;
    float aspectRat = m_screenWidth / m_screenHeight;
    float slot1_1 = 1 / (aspectRat * tan(fovRad / 2));
    float slot2_2 = 1 / tan(fovRad / 2);
    float slot3_3 = m_farClip / (m_farClip - m_nearClip);
    float slot3_4 = - m_farClip * m_nearClip / (m_farClip - m_nearClip);
    return mat4(vec4(slot1_1, 0, 0, 0),
                vec4(0, slot2_2, 0, 0),
                vec4(0, 0, slot3_3, 1),
                vec4(0, 0, slot3_4, 0));
}

Ray Camera::rayCast(Point2f pixelCo)
{
    vec4 worldCo = vec4(pixelCo.x, pixelCo.y, 1.f, 1.f);

    //from pixel space to homogenized screen space
    worldCo.x = (worldCo.x / m_screenWidth) * 2 - 1;
    worldCo.y = 1 - (worldCo.y / m_screenHeight) * 2;

    //from homogenized screen to unhomogenized screen
    worldCo *= m_farClip;

    //from unhomogenized to world space
    worldCo = m_viewProjectionMxInv * worldCo;

    return Ray(m_eye, normalize(vec3(worldCo.x, worldCo.y, worldCo.z) - m_eye));
}


