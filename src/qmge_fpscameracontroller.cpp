#include "qmge_fpscameracontroller.h"

namespace QMGE_Core
{

QMGE_FPSCameraController::QMGE_FPSCameraController(QMGE_GLCamera * camera)
{
    if(camera == nullptr)
    {
        qFatal("Can not set camera controller to null camera.");
    }
    else
    {
        m_camera = camera;
    }
    m_currentAngleSpeed = 0.0f;
    m_destRot = QVector3D(camera->getPitch(),camera->getYaw(),camera->getRoll());
    m_maxLinearspeed = 10.0f;
    m_maxAngleSpeed = 90.0f;
    m_angleAcc = 90.0f;
    m_linearAcc = 20.0f;

    m_isForward = false;
    m_isBack = false;
    m_isLeft = false;
    m_isRight = false;
    m_isUp = false;
    m_isDown = false;
}

void QMGE_FPSCameraController::updateCam(float deltaTime)
{
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
        m_currentLinearSpeed.length()<m_maxLinearspeed?(m_currentLinearSpeed+=QVector3D::crossProduct(m_camera->getUp(),m_camera->getForward())*m_linearAcc*deltaTime):(m_currentLinearSpeed=QVector3D::crossProduct(m_camera->getUp(),m_camera->getForward())*m_maxLinearspeed);
        moved = true;
    }
    if(m_isRight)
    {
        m_camera->setPosition(m_camera->getTransform().position + m_currentLinearSpeed * deltaTime);
        m_currentLinearSpeed.length()<m_maxLinearspeed?(m_currentLinearSpeed-=QVector3D::crossProduct(m_camera->getUp(),m_camera->getForward())*m_linearAcc*deltaTime):(m_currentLinearSpeed=QVector3D::crossProduct(m_camera->getUp(),m_camera->getForward())*(-m_maxLinearspeed));
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



    m_camera->setPitch(m_destRot.x());
    m_camera->setYaw(m_destRot.y());
    m_camera->pitch(m_destRot.x() - m_camera->getPitch());
    m_camera->yaw(m_destRot.y() - m_camera->getYaw());
    /*
    float deltax,deltay,deltaz;
    deltax = m_destRot.x() - m_camera->getPitch();
    deltay = m_destRot.y() - m_camera->getYaw();
    deltaz = m_destRot.z() - m_camera->getRoll();

    moved = false;
    if(deltax>0)
    {
        if(deltax < deltaTime * m_currentAngleSpeed)
        {
            m_camera->setPitch(m_camera->getPitch() + deltax);
            m_currentAngleSpeed = deltax;
        }
        else m_camera->setPitch(m_camera->getPitch() + deltaTime * m_currentAngleSpeed);
        m_currentAngleSpeed += m_angleAcc * deltaTime;
        moved = true;
    }
    else
    {
        if(deltax != 0) moved = true;
        if(deltax > (-deltaTime) * m_currentAngleSpeed)
        {
            m_camera->setPitch(m_camera->getPitch() + deltax);
            m_currentAngleSpeed = -deltax;
        }
        else m_camera->setPitch(m_camera->getPitch() - deltaTime * m_currentAngleSpeed);
        m_currentAngleSpeed += m_angleAcc * deltaTime;
    }

    if(deltay>0)
    {
        if(deltay < deltaTime * m_currentAngleSpeed)
        {
            m_camera->setYaw(m_camera->getYaw() + deltay);
            m_currentAngleSpeed = deltax;
        }
        else m_camera->setYaw(m_camera->getYaw() + deltaTime * m_currentAngleSpeed);
        m_currentAngleSpeed += m_angleAcc * deltaTime;
        moved = true;
    }
    else
    {
        if(deltax != 0) moved = true;
        if(deltay > (-deltaTime) * m_currentAngleSpeed)
        {
            m_camera->setYaw(m_camera->getYaw() + deltay);
            m_currentAngleSpeed = -deltax;
        }
        else m_camera->setPitch(m_camera->getYaw() - deltaTime * m_currentAngleSpeed);
        m_currentAngleSpeed += m_angleAcc * deltaTime;
    }

    if(deltaz>0)
    {
        if(deltaz < deltaTime * m_currentAngleSpeed)
        {
            m_camera->setRoll(m_camera->getRoll() + deltaz);
            m_currentAngleSpeed = deltax;
        }
        else m_camera->setRoll(m_camera->getRoll() + deltaTime * m_currentAngleSpeed);
        m_currentAngleSpeed += m_angleAcc * deltaTime;
        moved = true;
    }
    else
    {
        if(deltax != 0) moved = true;
        if(deltaz > (-deltaTime) * m_currentAngleSpeed)
        {
            m_camera->setRoll(m_camera->getRoll() + deltaz);
            m_currentAngleSpeed = -deltax;
        }
        else m_camera->setRoll(m_camera->getRoll() - deltaTime * m_currentAngleSpeed);
        m_currentAngleSpeed += m_angleAcc * deltaTime;
    }

    if(!moved) m_currentAngleSpeed = 0.0f;
    */

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
    case Qt::Key_Down:
        m_isDown = start;
        break;
    case Qt::Key_A:
        m_isLeft = start;
        break;
    case Qt::Key_D:
        m_isRight = start;
        break;
    default:
        break;
    }
}

void QMGE_FPSCameraController::rotate(QVector3D rot)
{
    m_destRot.setX(m_destRot.x() + rot.x());
    m_destRot.setY(m_destRot.y() + rot.y());
    m_destRot.setZ(m_destRot.z() + rot.z());
}

void QMGE_FPSCameraController::rotate(float x,float y,float z)
{
    m_destRot.setX(m_destRot.x() + x);
    m_destRot.setY(m_destRot.y() + y);
    m_destRot.setZ(m_destRot.z() + z);
}


}
//namespace QMGE_Core
