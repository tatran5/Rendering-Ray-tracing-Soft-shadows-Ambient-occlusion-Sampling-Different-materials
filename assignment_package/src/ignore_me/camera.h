#pragma once
#include "la.h"

class Camera
{
public:
    Camera();

    glm::mat4 getViewProj();
    void rotateTheta(int deg);
    void rotatePhi(int deg);
    void setAspect(int w, int h);
    void reset();

private:
    float aspect;
    float theta, phi; // For use in polar spherical camera model
};
