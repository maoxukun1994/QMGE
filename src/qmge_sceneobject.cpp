#include "qmge_sceneobject.h"

namespace QMGE_Core
{

QMGE_SceneObject::QMGE_SceneObject(QMGE_SceneObject * parent) : QObject(parent)
{
}

QMGE_Transform QMGE_SceneObject::getTransform()
{
    return m_transform;
}

//set
void QMGE_SceneObject::setTransform(QMGE_Transform &transform)
{
    m_transform = transform;
}

void QMGE_SceneObject::setPosition(QVector3D &position)
{
    m_transform.position = position;
}

void QMGE_SceneObject::setPosition(float x,float y,float z)
{
    m_transform.position = QVector3D(x,y,z);
}

void QMGE_SceneObject::setRotation(QQuaternion &rotation)
{
    m_transform.rotation = rotation;
}

void QMGE_SceneObject::setScale(QVector3D &scale)
{
    m_transform.scale = scale;
}

void QMGE_SceneObject::setScale(float x,float y,float z)
{
    m_transform.scale = QVector3D(x,y,z);
}


}
//namespace QMGE_Core
