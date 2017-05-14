#ifndef QMGE_GLCAMERA_H
#define QMGE_GLCAMERA_H

#include "qmge_global.h"
#include "qmge_sceneobject.h"

namespace QMGE_Core
{

enum QMGE_CameraPerspective
{
    PERSPECTIVE = 0,
    ORTHO
};

class QMGE_GLCamera : public QMGE_SceneObject
{

public:

    QMGE_GLCamera(QMGE_CameraPerspective type = PERSPECTIVE,QMGE_SceneObject * parent = nullptr);
    ~QMGE_GLCamera();

    //type
    void setPerspective(float fov,float aspect,float near,float far);
    void setOrtho(float left, float right, float bottom, float top, float nearPlane, float farPlane);

    //get
    QMatrix4x4 getVMatrix();
    QMatrix4x4 getPMatrix();

    float getPitch();
    float getYaw();
    float getRoll();

    QVector3D getForward();
    QVector3D getUp();

    //set
    void setPitch(float degree);
    void setYaw(float degree);
    void setRoll(float degree);

    //camera rotation
    void pitch(float degree);
    void yaw(float degree);
    void roll(float degree);

    //update
    void updateV();
    void updateP();

public:

    QMatrix4x4 m_vMatrix;
    QMatrix4x4 m_pMatrix;

protected:

    QMGE_CameraPerspective m_type;

    const QVector3D baseForward;
    const QVector3D baseUp;
    const QVector3D baseLeft;

    QVector3D m_forward;
    QVector3D m_up;

    //control
    float m_pitchDegree;
    float m_yawDegree;
    float m_rollDegree;

    //for distance clipping
    float m_near;
    float m_far;
    //perspective
    float m_fov;
    float m_aspect;
    //ortho
    float m_left;
    float m_right;
    float m_bottom;
    float m_top;
};
//class QMGE_GLCamera


}
//QMGE_Core

#endif // QMGE_GLCAMERA_H
