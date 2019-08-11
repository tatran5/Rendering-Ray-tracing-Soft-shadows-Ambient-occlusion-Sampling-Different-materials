#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "globalincludes.h"

/* A Transform will represent the sequence of transformations that
 *  have been applied to a primitive in the scene in order to transform it
 *  to its current location, size, and orientation. */
class Transform
{
public:

    Vector3f m_trans;
    Vector3f m_scale;
    Vector3f m_angleRotate;
    glm::mat4 m_worldTransform;
    glm::mat4 m_worldTransformInv;
    glm::mat3 m_worldTransformInvTranspose;

    Transform();
    Transform(glm::vec3 t, glm::vec3 r, glm::vec3 s);

    void setMemberVars(); //calculate and set m_worldTransform, m_worldTransformInv, m_worldTransformInvTranspose
};

#endif // TRANSFORM_H
