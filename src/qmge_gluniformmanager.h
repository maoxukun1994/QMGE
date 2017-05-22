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

class QMGE_HostUniform
{

public:

    QMGE_UniformType type;

    void * dataPtr;

    int refCount;

    QMGE_HostUniform(QMGE_UniformType utype = NOTYPE):type(utype),dataPtr(nullptr),refCount(0){}

};

class QMGE_ShaderUniform
{

public:

    QString name;

    QMGE_UniformType type;

    GLint location;

    void * dataPtr;

    QMGE_ShaderUniform():name(""),type(NOTYPE),location(0),dataPtr(nullptr){}
};


//the uniform manager needs to be thread-safe
//caution:
//the design propose of this class is to provide a manageable uniform set
//but not to assure the status of uniform data pointers.
//the class will NOT take over ownership of any dataPointer of MGE_HostUniforms
//so it's other classes' responsibility to assure they don't change the pointer,or
//called proper functions(such as unregister..etc.)to inform the class that the uniform pointer
//has changed.

class QMGE_GLUniformManager
{

public:

    ~QMGE_GLUniformManager();

    static QMGE_GLUniformManager * getInstance();

    template<typename T> bool registerUniform(QString name,T type,T * &provider)
    {
        bool ret = false;
        //lock hash table
        QMutexLocker locker(&m_hashTableLock);
        //try find item
        auto item = m_uniforms.find(name);
        if(item != m_uniforms.end())
        {
            //found dumplicate name,not register,just set provider
            provider = reinterpret_cast<T *>(item.value().dataPtr);
        }
        else
        {
            //item not found.Register a new one.
            //make new HostUniform class
            QMGE_HostUniform newUniform;
            //check and decide type
            if( typeid(type) == typeid(int) ) newUniform.type = INT;
            if( typeid(type) == typeid(float) ) newUniform.type = FLOAT;
            if( typeid(type) == typeid(QVector2D) ) newUniform.type = VEC2;
            if( typeid(type) == typeid(QVector3D) ) newUniform.type = VEC3;
            if( typeid(type) == typeid(QVector4D) ) newUniform.type = VEC4;
            if( typeid(type) == typeid(QMatrix3x3) ) newUniform.type = MAT3;
            if( typeid(type) == typeid(QMatrix4x4) ) newUniform.type = MAT4;

            //if is not one of these types above
            if(newUniform.type == NOTYPE)
            {
                qCritical("Can not register uniform.Unknown type.");
                //and ret will be a nullptr
            }
            else
            {
                //allocate memory for this uniform
                provider = new T(type);
                //set data pointer
                newUniform.dataPtr = reinterpret_cast<void *>(provider);
                newUniform.refCount = 0;
            }
            //save
            m_uniforms.insert(name,newUniform);

            ret = true;
        }
        return ret;
    }

    bool unregisterUniform(QString name);

    bool bindShaderUniform(QMGE_ShaderUniform &uniform);

    bool unbindShaderUniform(QMGE_ShaderUniform &uniform);

    QMGE_HostUniform getUniform(QString name);

private:

    QMGE_GLUniformManager();

    void deletePtr(QMGE_HostUniform &p);

private:

    QHash<QString,QMGE_HostUniform> m_uniforms;

    QMutex m_hashTableLock;
};

//class QMGE_GLUniformManager


}
//namespace QMGE_Core

#endif // QMGE_GLUNIFORMMANAGER_H
