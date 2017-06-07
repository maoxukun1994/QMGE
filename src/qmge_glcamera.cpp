#include "qmge_glcamera.h"

namespace QMGE_Core
{

QMGE_GLCamera::QMGE_GLCamera(QMGE_CameraPerspective type, QMGE_SceneObject * parent):
QMGE_SceneObject(parent),baseForward(1.0f,0.0f,0.0f),baseUp(0.0f,0.0f,1.0f),baseLeft(0.0f,1.0f,0.0f)
{
    m_type = type;

    m_pitchDegree = 0.0f;
    m_yawDegree = 0.0f;
    m_rollDegree = 0.0f;

    m_forward = baseForward;
    m_up = baseUp;
    m_left = baseLeft;

    //for distance clipping
    m_near = 0.1f;
    m_far = 100.0f;
    //perspective
    m_fov = 60.0f;
    m_aspect = 1.788f;
    //ortho
    m_oleft = 0.0f;
    m_oright = 0.0f;
    m_obottom = 0.0f;
    m_otop = 0.0f;

    registerUniforms();
}

QMGE_GLCamera::~QMGE_GLCamera()
{

}

void QMGE_GLCamera::registerUniforms()
{
    bool status = true;
    status = status && QMGE_GLUniformManager::getInstance()->registerUniform("vMatrix",QMatrix4x4(),m_vMatrix);
    status = status && QMGE_GLUniformManager::getInstance()->registerUniform("pMatrix",QMatrix4x4(),m_pMatrix);
    if(!status)
    {
        qFatal("camera uniform register failed.");
    }
}

//type
void QMGE_GLCamera::setPerspective(float fov,float aspect,float vnear,float vfar)
{
    m_type = QMGE_CameraPerspective::PERSPECTIVE;
    m_fov = fov;
    m_aspect = aspect;
    m_near = vnear;
    m_far = vfar;
    updateP();
}

void QMGE_GLCamera::setOrtho(float left, float right, float bottom, float top, float nearPlane, float farPlane)
{
    m_type = QMGE_CameraPerspective::ORTHO;
    m_oleft = left;
    m_oright = right;
    m_obottom = bottom;
    m_otop = top;
    m_near = nearPlane;
    m_far = farPlane;
    updateP();
}

//get
QMatrix4x4 * QMGE_GLCamera::getVMatrix()
{
    return m_vMatrix;
}

QMatrix4x4 * QMGE_GLCamera::getPMatrix()
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

QVector3D QMGE_GLCamera::getLeft()
{
    return m_left;
}

void QMGE_GLCamera::setPitch(float degree)
{
    m_pitchDegree = degree;
}

void QMGE_GLCamera::setYaw(float degree)
{
    m_yawDegree = degree;
}

void QMGE_GLCamera::setRoll(float degree)
{
    m_rollDegree = degree;
}

void QMGE_GLCamera::pitch(float degree)
{
    m_pitchDegree += degree;
    if(m_pitchDegree > 85.0f) m_pitchDegree = 85.0f;
    if(m_pitchDegree < -85.0f) m_pitchDegree = -85.0f;
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
    m_vMatrix->setToIdentity();

    //including pitch and yaw
    m_forward.setZ(qSin(qDegreesToRadians(m_pitchDegree)));
    m_forward.setX(qCos(qDegreesToRadians(m_pitchDegree)) * qCos(qDegreesToRadians(m_yawDegree)));
    m_forward.setY(qCos(qDegreesToRadians(m_pitchDegree)) * qSin(qDegreesToRadians(m_yawDegree)));
    m_forward.normalize();

    //roll
    m_up.setX(qSin(qDegreesToRadians(m_rollDegree)) * qSin(qDegreesToRadians(m_yawDegree)));
    m_up.setY(qSin(qDegreesToRadians(m_rollDegree)) * qCos(qDegreesToRadians(m_yawDegree)));
    m_up.setZ(qCos(qDegreesToRadians(m_rollDegree)));
    m_up.normalize();

    m_left = QVector3D::crossProduct(m_up,m_forward).normalized();

    m_vMatrix->lookAt(m_transform.position,m_transform.position+m_forward,m_up);
}

void QMGE_GLCamera::updateP()
{
    m_pMatrix->setToIdentity();
    switch(m_type)
    {
    case QMGE_CameraPerspective::PERSPECTIVE:
        m_pMatrix->perspective(m_fov,m_aspect,m_near,m_far);
        break;
    case QMGE_CameraPerspective::ORTHO:
        m_pMatrix->ortho(m_oleft,m_oright,m_obottom,m_otop,m_near,m_far);
        break;
    }
}

}
//namespace QMGE_Core
