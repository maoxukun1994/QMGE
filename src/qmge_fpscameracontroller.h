#ifndef QMGE_FPSCAMERACONTROLLER_H
#define QMGE_FPSCAMERACONTROLLER_H

#include "qmge_global.h"
#include "qmge_glcamera.h"

namespace QMGE_Core
{

enum QMGE_MoveDirection
{
    FORWARD = 0,
    BACK,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    BOOST,
    YAW_LEFT,
    YAW_RIGHT,
    PITCH_UP,
    PITCH_DOWN,
    ROLL_LEFT,
    ROLL_RIGHT,
};

class QMGE_FPSCameraController : public QObject
{
    Q_OBJECT

public:

    QMGE_FPSCameraController(QSharedPointer<QMGE_GLCamera> cam);

    void updateCam(float deltaTime);

    void setMaxLinearSpeed(float speed);
    //in degrees
    void setMaxAngleSpeed(float speed);

    void stop();

    QVector3D getLinearVelocity();

public slots:
    //move
    void move(QMGE_MoveDirection dir,bool start);
    //rotate overloads for fps
    void rotate(QVector3D rot);
    void rotate(float x,float y,float z);
    void rotateTo(QVector3D dest);
    void rotateTo(float x,float y,float z);

private:

    void checkDestRotate();

private:

    QSharedPointer<QMGE_GLCamera> m_camera;

    //linear speeed
    QVector3D m_currentLinearSpeed;
    float m_maxLinearspeed;
    float m_linearAcc;

    //rotate speed
    //m_destRot,x as yaw,y as pitch,z as roll
    QVector3D m_destRot;
    float m_maxAngleSpeed;
    float m_rotateSpeedScale;


    bool m_isForward;
    bool m_isBack;
    bool m_isLeft;
    bool m_isRight;
    bool m_isUp;
    bool m_isDown;
    bool m_isRollingLeft;
    bool m_isRollingRight;

    bool m_turbo;
};
//class QMGE_FPSCameraController


}
//namespace QMGE_Core

#endif // QMGE_FPSCAMERACONTROLLER_H
