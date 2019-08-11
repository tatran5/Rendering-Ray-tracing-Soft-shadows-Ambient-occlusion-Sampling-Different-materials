#include "camera.h"

Camera::Camera()
    : aspect(1), theta(0), phi(0)
{}

glm::mat4 Camera::getViewProj()
{
    glm::mat4 rot = glm::rotate(glm::mat4(), glm::radians(theta), glm::vec3(0,1,0)) * glm::rotate(glm::mat4(), glm::radians(phi), glm::vec3(1,0,0));
    glm::vec3 eye = glm::vec3(rot * glm::vec4(0, 0, 1.5f, 1));
    glm::vec3 up = glm::vec3(rot * glm::vec4(0,1,0,0));
    return glm::perspective(glm::radians(45.f), aspect, 0.01f, 10.f) * glm::lookAt(eye, glm::vec3(0,0,0), up);
}

void Camera::rotateTheta(int deg)
{
    theta += deg;
}

void Camera::rotatePhi(int deg)
{
    phi += deg;
}

void Camera::setAspect(int w, int h)
{
    aspect = w / (float)(h);
}

void Camera::reset()
{
    theta = phi = 0;
}
