#ifndef QMGE_SCENEOBJECT_H
#define QMGE_SCENEOBJECT_H

#include "qmge_global.h"

namespace QMGE_Core
{

//all directional-vectors in should be normalized
typedef struct qmgetransform
{
    //global position
    QVector3D position;
    //global rotation
    QQuaternion rotation;
    //global scale
    QVector3D scale;
}
QMGE_Transform;

class QMGE_SceneObject : public QObject
{
    Q_OBJECT

public:

    explicit QMGE_SceneObject(QMGE_SceneObject * parent = nullptr);

    QMGE_Transform getTransform();

    //set
    void setTransform(QMGE_Transform transform);
    void setPosition(QVector3D position);
    void setPosition(float x,float y,float z);
    void setRotation(QQuaternion rotation);
    void setScale(QVector3D &scale);
    void setScale(float x,float y,float z);

protected:

    QMGE_Transform m_transform;

};
//class QMGE_SceneObject


}
//namespace QMGE_Core


#endif // QMGE_SCENEOBJECT_H
