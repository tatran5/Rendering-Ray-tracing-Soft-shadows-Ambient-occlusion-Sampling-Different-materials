#include "transform.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <math.h>

Transform::Transform() : m_trans(0.f, 0.f, 0.f), m_angleRotate(0.f, 0.f, 0.f), m_scale(1.f, 1.f, 1.f)
{
    setMemberVars();
}

Transform::Transform(glm::vec3 t, glm::vec3 r, glm::vec3 s) : m_trans(t), m_angleRotate(r), m_scale(s)
{
    setMemberVars();
}

void Transform::setMemberVars() {
    m_worldTransform = glm::translate(glm::mat4(), m_trans) *
            glm::rotate(m_angleRotate.x, glm::vec3(1, 0, 0)) *
            glm::rotate(m_angleRotate.y, glm::vec3(0, 1, 0)) *
            glm::rotate(m_angleRotate.z, glm::vec3(0, 0, 1)) *
            glm::scale(glm::mat4(), m_scale);
    m_worldTransformInv = glm::inverse(m_worldTransform);
    //get the columns of worldTransformInv
    glm::vec4 row0 = glm::row(m_worldTransformInv, 0);
    glm::vec4 row1 = glm::row(m_worldTransformInv, 1);
    glm::vec4 row2 = glm::row(m_worldTransformInv, 2);
    m_worldTransformInvTranspose = glm::mat3(glm::vec3(row0.x, row0.y, row0.z),
                                        glm::vec3(row1.x, row1.y, row1.z),
                                        glm::vec3(row2.x, row2.y, row2.z));
}
