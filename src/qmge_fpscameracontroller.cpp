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
    m_camera = camera;

    m_destRot = QVector3D(camera->getYaw(),camera->getPitch(),camera->getRoll());

    m_maxLinearspeed = 10.0f;
    m_linearAcc = 3.0f;

    m_maxAngleSpeed = 360.0f;
    m_rotateSpeedScale = 20.0f;

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

    auto cameraPos = m_camera->getTransform().position;
    //position
    if(m_isForward)
    {
        m_currentLinearSpeed += m_camera->getForward() * m_linearAcc * deltaTime;
        moved = true;
    }
    if(m_isBack)
    {
        m_currentLinearSpeed -= m_camera->getForward() * m_linearAcc * deltaTime;
        moved = true;
    }
    if(m_isLeft)
    {
        m_currentLinearSpeed += m_camera->getLeft() * m_linearAcc * deltaTime;
        moved = true;
    }
    if(m_isRight)
    {
        m_currentLinearSpeed -= m_camera->getLeft() * m_linearAcc * deltaTime;
        moved = true;
    }
    if(m_isUp)
    {
        m_currentLinearSpeed += m_camera->getUp() * m_linearAcc * deltaTime;
        moved = true;
    }
    if(m_isDown)
    {
        m_currentLinearSpeed -= m_camera->getUp() * m_linearAcc * deltaTime;
        moved = true;
    }

    //limit speed
    if(m_currentLinearSpeed.length() > m_maxLinearspeed)
    {
        m_currentLinearSpeed /= (m_currentLinearSpeed.length()/m_maxLinearspeed);
    }

    if(!moved)
    {
        m_currentLinearSpeed.length()<0.05?m_currentLinearSpeed=QVector3D(0,0,0):(m_currentLinearSpeed*=0.95);
    }

    m_camera->setPosition(cameraPos + m_currentLinearSpeed * deltaTime);


    //rotation
    float deltaYaw = m_destRot.x() - m_camera->getYaw();
    if(qAbs(deltaYaw) > 180.0f)
    {
        if(deltaYaw > 0) deltaYaw -= 360.0f;
        else deltaYaw += 360.0f;
    }

    float deltaPitch = m_destRot.y() - m_camera->getPitch();

    float deltaRoll = m_destRot.z() - m_camera->getRoll();
    if(qAbs(deltaRoll) > 180.0f)
    {
        if(deltaRoll > 0) deltaRoll -= 360.0f;
        else deltaRoll += 360.0f;
    }

    deltaYaw /= m_rotateSpeedScale;
    deltaPitch /= m_rotateSpeedScale;
    deltaRoll /= m_rotateSpeedScale;

    QVector3D rotSpd(deltaYaw,deltaPitch,deltaRoll);
    if(rotSpd.length() > m_maxAngleSpeed * deltaTime)
    {
        rotSpd /= (rotSpd.length()/m_maxAngleSpeed/deltaTime);
    }

    //change rot
    m_camera->yaw(rotSpd.x());
    m_camera->pitch(rotSpd.y());
    m_camera->roll(rotSpd.z());

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

void QMGE_FPSCameraController::move(QMGE_MoveDirection dir, bool start)
{
    switch (dir)
    {
    case FORWARD:
        m_isForward = start;
        break;
    case BACK:
        m_isBack = start;
        break;
    case UP:
        m_isUp = start;
        break;
    case DOWN:
        m_isDown = start;
        break;
    case LEFT:
        m_isLeft = start;
        break;
    case RIGHT:
        m_isRight = start;
        break;
    case BOOST:
        m_turbo = start;
    default:
        break;
    }
}

void QMGE_FPSCameraController::rotate(QVector3D rot)
{
    m_destRot += rot;
    checkDestRotate();
}

void QMGE_FPSCameraController::rotate(float x,float y,float z)
{
    m_destRot += QVector3D(x,y,z);
    checkDestRotate();
}

void QMGE_FPSCameraController::rotateTo(QVector3D dest)
{
    m_destRot = dest;
    checkDestRotate();
}

void QMGE_FPSCameraController::rotateTo(float x,float y,float z)
{
    m_destRot = QVector3D(x,y,z);
    checkDestRotate();
}

void QMGE_FPSCameraController::checkDestRotate()
{
    if(m_destRot.x() > 360)
    {
        m_destRot.setX(m_destRot.x()-360);
    }
    if(m_destRot.x() < 0)
    {
        m_destRot.setX(m_destRot.x()+360);
    }
    if(m_destRot.y() > 85)
    {
        m_destRot.setY(85);
    }
    if(m_destRot.y() < -85)
    {
        m_destRot.setY(-85);
    }
    if(m_destRot.z() > 360)
    {
        m_destRot.setZ(m_destRot.z()-360);
    }
    if(m_destRot.z() < 0)
    {
        m_destRot.setZ(m_destRot.z()+360);
    }
}


}
//namespace QMGE_Core
