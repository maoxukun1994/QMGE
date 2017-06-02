#ifndef QMGE_FPSCAMERACONTROLLER_H
#define QMGE_FPSCAMERACONTROLLER_H

#include "qmge_global.h"
#include "qmge_glcamera.h"

namespace QMGE_Core
{

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

    void move(int keyCode,bool start);

    void rotate(QVector3D rot);
    //rotate overload for fps
    void rotate(float x,float y,float z);

private:

    QSharedPointer<QMGE_GLCamera> m_camera;

    QVector3D m_currentLinearSpeed;
    float m_currentAngleSpeed;
    float m_maxLinearspeed;
    float m_maxAngleSpeed;
    float m_angleAcc;
    float m_linearAcc;

    bool m_isForward;
    bool m_isBack;
    bool m_isLeft;
    bool m_isRight;
    bool m_isUp;
    bool m_isDown;

    bool m_turbo;
};
//class QMGE_FPSCameraController


}
//namespace QMGE_Core

#endif // QMGE_FPSCAMERACONTROLLER_H
