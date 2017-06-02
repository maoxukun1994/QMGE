#include "qmge_fpscameracontroller.h"

namespace QMGE_Core
{

QMGE_FPSCameraController::QMGE_FPSCameraController(QSharedPointer<QMGE_GLCamera> camera)
{
    if(camera.isNull())
    {
        qWarning("Can not set camera controller to null camera.");
        camera.reset(new QMGE_GLCamera());
    }
    else
    {
        m_camera = camera;
    }
    m_currentAngleSpeed = 0.0f;
    m_maxLinearspeed = 10.0f;
    m_maxAngleSpeed = 90.0f;
    m_angleAcc = 90.0f;
    m_linearAcc = 3.0f;

    m_isForward = false;
    m_isBack = false;
    m_isLeft = false;
    m_isRight = false;
    m_isUp = false;
    m_isDown = false;

    m_turbo = false;
}

void QMGE_FPSCameraController::updateCam(float deltaTime)
{
    if(m_turbo) deltaTime *= 3;

    bool moved = false;
    //position
    if(m_isForward)
    {
        m_camera->setPosition(m_camera->getTransform().position + m_currentLinearSpeed * deltaTime);
        m_currentLinearSpeed.length()<m_maxLinearspeed?(m_currentLinearSpeed+=m_camera->getForward()*m_linearAcc*deltaTime):(m_currentLinearSpeed=m_camera->getForward()*m_maxLinearspeed);
        moved = true;
    }
    if(m_isBack)
    {
        m_camera->setPosition(m_camera->getTransform().position + m_currentLinearSpeed * deltaTime);
        m_currentLinearSpeed.length()<m_maxLinearspeed?(m_currentLinearSpeed-=m_camera->getForward()*m_linearAcc*deltaTime):(m_currentLinearSpeed=m_camera->getForward()*(-m_maxLinearspeed));
        moved = true;
    }
    if(m_isLeft)
    {
        m_camera->setPosition(m_camera->getTransform().position + m_currentLinearSpeed * deltaTime);
        m_currentLinearSpeed.length()<m_maxLinearspeed?(m_currentLinearSpeed+=QVector3D::crossProduct(m_camera->getUp(),m_camera->getForward())*m_linearAcc*deltaTime):(m_currentLinearSpeed=QVector3D::crossProduct(m_camera->getUp(),m_camera->getForward()).normalized()*m_maxLinearspeed);
        moved = true;
    }
    if(m_isRight)
    {
        m_camera->setPosition(m_camera->getTransform().position + m_currentLinearSpeed * deltaTime);
        m_currentLinearSpeed.length()<m_maxLinearspeed?(m_currentLinearSpeed-=QVector3D::crossProduct(m_camera->getUp(),m_camera->getForward())*m_linearAcc*deltaTime):(m_currentLinearSpeed=QVector3D::crossProduct(m_camera->getUp(),m_camera->getForward()).normalized()*(-m_maxLinearspeed));
        moved = true;
    }
    if(m_isUp)
    {
        m_camera->setPosition(m_camera->getTransform().position + m_currentLinearSpeed * deltaTime);
        m_currentLinearSpeed.length()<m_maxLinearspeed?(m_currentLinearSpeed+=m_camera->getUp()*m_linearAcc*deltaTime):(m_currentLinearSpeed=m_camera->getUp()*m_maxLinearspeed);
        moved = true;
    }
    if(m_isDown)
    {
        m_camera->setPosition(m_camera->getTransform().position + m_currentLinearSpeed * deltaTime);
        m_currentLinearSpeed.length()<m_maxLinearspeed?(m_currentLinearSpeed-=m_camera->getUp()*m_linearAcc*deltaTime):(m_currentLinearSpeed=m_camera->getUp()*(-m_maxLinearspeed));
        moved = true;
    }

    if(!moved)
    {
        m_currentLinearSpeed.length()<0.1?m_currentLinearSpeed=QVector3D(0,0,0):(m_currentLinearSpeed*=0.9);
        m_camera->setPosition(m_camera->getTransform().position + m_currentLinearSpeed * deltaTime);
    }

    m_camera->updateV();
}

void QMGE_FPSCameraController::setMaxLinearSpeed(float speed)
{
    m_maxLinearspeed = speed;
}

//in degrees
void QMGE_FPSCameraController::setMaxAngleSpeed(float speed)
{
    m_maxAngleSpeed = speed;
}

void QMGE_FPSCameraController::stop()
{
    m_currentLinearSpeed = QVector3D();
}

QVector3D QMGE_FPSCameraController::getLinearVelocity()
{
    return m_currentLinearSpeed;
}

void QMGE_FPSCameraController::move(int keyCode,bool start)
{
    switch (keyCode)
    {
    case Qt::Key_W:
        m_isForward = start;
        break;
    case Qt::Key_S:
        m_isBack = start;
        break;
    case Qt::Key_Up:
        m_isUp = start;
        break;
    case Qt::Key_Q:
        m_isUp = start;
        break;
    case Qt::Key_Down:
        m_isDown = start;
        break;
    case Qt::Key_E:
        m_isDown = start;
        break;
    case Qt::Key_A:
        m_isLeft = start;
        break;
    case Qt::Key_D:
        m_isRight = start;
        break;
    case Qt::Key::Key_Shift:
        m_turbo = start;
    default:
        break;
    }
}

void QMGE_FPSCameraController::rotate(QVector3D rot)
{
    m_camera->pitch(rot.x());
    m_camera->yaw(rot.y());
    m_camera->roll(rot.z());
}

void QMGE_FPSCameraController::rotate(float x,float y,float z)
{
    m_camera->pitch(x);
    m_camera->yaw(y);
    m_camera->roll(z);
}


}
//namespace QMGE_Core
