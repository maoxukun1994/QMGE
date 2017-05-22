#include "qmge_gluniformmanager.h"

namespace QMGE_Core
{

QMGE_GLUniformManager::~QMGE_GLUniformManager()
{
    //cleaning
    for(auto p : m_uniforms)
    {
        deletePtr(p);
    }
}

QMGE_GLUniformManager * QMGE_GLUniformManager::getInstance()
{
    static QMGE_GLUniformManager m_instance;
    return &m_instance;
}

bool QMGE_GLUniformManager::unregisterUniform(QString name)
{
    bool ret = false;
    //lock hash table
    QMutexLocker locker(&m_hashTableLock);
    //try find item
    auto item = m_uniforms.find(name);
    //found
    if(item != m_uniforms.end())
    {
        if(item.value().refCount == 0)
        {
            deletePtr((item.value()));
            m_uniforms.erase(item);
            ret = true;
        }
    }
    return ret;
}

bool QMGE_GLUniformManager::bindShaderUniform(QMGE_ShaderUniform &uniform)
{
    bool ret = false;
    //lock hash table
    QMutexLocker locker(&m_hashTableLock);
    //try find item
    auto item = m_uniforms.find(uniform.name);
    //found
    if(item != m_uniforms.end())
    {
        //check if type dismatch
        if( uniform.type == item.value().type )
        {
            //link pointer
            uniform.dataPtr = item.value().dataPtr;
            item.value().refCount += 1;
            ret = true;
        }
    }
    return ret;
}

bool QMGE_GLUniformManager::unbindShaderUniform(QMGE_ShaderUniform &uniform)
{
    bool ret = false;
    //lock hash table
    QMutexLocker locker(&m_hashTableLock);
    //try find item
    auto item = m_uniforms.find(uniform.name);
    //found
    if(item != m_uniforms.end())
    {
        //unlink pointer
        uniform.dataPtr = nullptr;
        item.value().refCount -= 1;
        ret = true;
    }
    return ret;
}

QMGE_HostUniform QMGE_GLUniformManager::getUniform(QString name)
{
    QMGE_HostUniform ret(NOTYPE);
    //lock hash table
    QMutexLocker locker(&m_hashTableLock);
    //try find item
    auto item = m_uniforms.find(name);
    //found
    if(item != m_uniforms.end())
    {
        ret = item.value();
    }
    return ret;
}


QMGE_GLUniformManager::QMGE_GLUniformManager()
{

}

void QMGE_GLUniformManager::deletePtr(QMGE_HostUniform &p)
{
    if(p.dataPtr != nullptr)
    {
        switch(p.type)
        {
        case INT:
            delete reinterpret_cast<int *>(p.dataPtr);
            break;
        case FLOAT:
            delete reinterpret_cast<float *>(p.dataPtr);
            break;
        case VEC2:
            delete reinterpret_cast<QVector2D *>(p.dataPtr);
            break;
        case VEC3:
            delete reinterpret_cast<QVector3D *>(p.dataPtr);
            break;
        case VEC4:
            delete reinterpret_cast<QVector4D *>(p.dataPtr);
            break;
        case MAT3:
            delete reinterpret_cast<QMatrix3x3 *>(p.dataPtr);
            break;
        case MAT4:
            delete reinterpret_cast<QMatrix4x4 *>(p.dataPtr);
            break;
        default:
            break;
        }
    }
}


}
//namespace QMGE_Core
