#include "qmge_glcamera.h"

namespace QMGE_Core
{

QMGE_GLCamera::QMGE_GLCamera(QMGE_CameraPerspective type, QMGE_SceneObject * parent) : QMGE_SceneObject(parent),
    baseForward(0.0f,1.0f,0.0f),baseUp(0.0f,0.0f,1.0f),baseLeft(-1.0f,0.0f,0.0f)
{
    m_type = type;

    m_pitchDegree = 0.0f;
    m_yawDegree = 90.0f;
    m_rollDegree = 0.0f;

    m_forward = baseForward;
    m_up = baseUp;

    //for distance clipping
    m_near = 0.1f;
    m_far = 100.0f;
    //perspective
    m_fov = 90.0f;
    m_aspect = 1.0f;
    //ortho
    m_left = 0.0f;
    m_right = 0.0f;
    m_bottom = 0.0f;
    m_top = 0.0f;
}

QMGE_GLCamera::~QMGE_GLCamera()
{

}

//type
void QMGE_GLCamera::setPerspective(float fov,float aspect,float near,float far)
{
    m_type = QMGE_CameraPerspective::PERSPECTIVE;
    m_fov = fov;
    m_aspect = aspect;
    m_near = near;
    m_far = far;

    updateP();
}

void QMGE_GLCamera::setOrtho(float left, float right, float bottom, float top, float nearPlane, float farPlane)
{
    m_type = QMGE_CameraPerspective::ORTHO;
    m_left = left;
    m_right = right;
    m_bottom = bottom;
    m_top = top;
    m_near = nearPlane;
    m_far = farPlane;

    updateP();
}

//get
QMatrix4x4 QMGE_GLCamera::getVMatrix()
{
    return m_vMatrix;
}

QMatrix4x4 QMGE_GLCamera::getPMatrix()
{
    return m_pMatrix;
}

float QMGE_GLCamera::getPitch()
{
    return m_pitchDegree;
}

float QMGE_GLCamera::getYaw()
{
    return m_yawDegree;
}

float QMGE_GLCamera::getRoll()
{
    return m_rollDegree;
}

QVector3D QMGE_GLCamera::getForward()
{
    return m_forward;
}

QVector3D QMGE_GLCamera::getUp()
{
    return m_up;
}

void QMGE_GLCamera::setPitch(float degree)
{
    m_pitchDegree = degree;
    updateV();
}

void QMGE_GLCamera::setYaw(float degree)
{
    m_yawDegree = degree;
    updateV();
}

void QMGE_GLCamera::setRoll(float degree)
{
    m_rollDegree = degree;
    updateV();
}

void QMGE_GLCamera::pitch(float degree)
{
    m_pitchDegree += degree;
    if(m_pitchDegree > 89.0f) m_pitchDegree = 89.0f;
    if(m_pitchDegree < -89.0f) m_pitchDegree = -89.0f;
}

void QMGE_GLCamera::yaw(float degree)
{
    m_yawDegree += degree;
    if(m_yawDegree > 360.0f) m_yawDegree -= 360.0f;
    if(m_yawDegree < 0.0f) m_yawDegree += 360.0f;
}

void QMGE_GLCamera::roll(float degree)
{
    m_rollDegree += degree;
    if(m_rollDegree > 360.0f) m_rollDegree -= 360.0f;
    if(m_rollDegree < 0.0f) m_rollDegree += 360.0f;
}

//update
void QMGE_GLCamera::updateV()
{
    m_vMatrix.setToIdentity();

    //including pitch and yaw, not including roll
    m_forward.setZ(qSin(qDegreesToRadians(m_pitchDegree)));
    m_forward.setX(qCos(qDegreesToRadians(m_pitchDegree)) * qCos(qDegreesToRadians(m_yawDegree)));
    m_forward.setY(qCos(qDegreesToRadians(m_pitchDegree)) * qSin(qDegreesToRadians(m_yawDegree)));
    m_forward.normalize();
    m_vMatrix.lookAt(m_transform.position,m_transform.position+m_forward,m_up);
}

void QMGE_GLCamera::updateP()
{
    m_pMatrix.setToIdentity();
    switch(m_type)
    {
    case QMGE_CameraPerspective::PERSPECTIVE:
        m_pMatrix.perspective(m_fov,m_aspect,m_near,m_far);
        break;
    case QMGE_CameraPerspective::ORTHO:
        m_pMatrix.ortho(m_left,m_right,m_bottom,m_top,m_near,m_far);
        break;
    }
}

}
//namespace QMGE_Core
