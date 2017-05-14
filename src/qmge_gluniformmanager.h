#ifndef QMGE_GLUNIFORMMANAGER_H
#define QMGE_GLUNIFORMMANAGER_H

#include "qmge_global.h"

namespace QMGE_Core
{

enum QMGE_UniformUpdateType
{
    FRAME_UPDATE = 0,
    FIX_UPDATE, //1
    ONCE_UPDATE //2
};

//uniform type
enum QMGE_UniformType
{
    NOTYPE = 0,
    INT, //1
    FLOAT, //2

    VEC2, //3
    VEC3, //4
    VEC4, //5

    MAT3, //6
    MAT4, //7
};

typedef struct qmgeuniform_h
{
    QMGE_UniformType type;
    void * dataPointer;
    unsigned int refCount;
    //constructor
    qmgeuniform_h(QMGE_UniformType utype = NOTYPE,void * udataPointer = nullptr):type(utype),dataPointer(udataPointer),refCount(0){}
}
QMGE_HostUniform;

typedef struct qmgeuniform_s
{
    QString name;
    QMGE_UniformType type;
    void * data;
    GLint location;
    //constructor
    qmgeuniform_s():name(""),type(QMGE_UniformType::NOTYPE),data(nullptr),location(0){}
}
QMGE_ShaderUniform;


//the uniform manager needs to be thread-safe
//caution:
//the design propose of this class is to provide a manageable uniform set
//but not to assure the status of uniform data pointers.
//the class will NOT take over ownership of any dataPointer of MGE_HostUniforms
//so it's other classes' responsibility to assure they don't change the pointer,or
//called proper functions(such as unregister..etc.)to inform the class that the uniform pointer
//has changed.
class QMGE_GLUniformManager : public QObject
{
    Q_OBJECT

public:

    ~QMGE_GLUniformManager();

    static QMGE_GLUniformManager * getInstance();

    bool registerHostUniform(QString name,QMGE_UniformType type,void * pointer);

    bool unregisterHostUniform(QString name);

    bool bindShaderUniform(QMGE_ShaderUniform &uniform);

    void unbindShaderUniform(QMGE_ShaderUniform &uniform);

    QMGE_HostUniform getUniform(QString name);

private:

    QMGE_GLUniformManager();

private:

    QHash<QString,QMGE_HostUniform> m_uniforms;

    QMutex m_uniformsWriteLock;
};

//class QMGE_GLUniformManager


}
//namespace QMGE_Core

#endif // QMGE_GLUNIFORMMANAGER_H
