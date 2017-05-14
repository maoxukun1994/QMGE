#include "qmge_gluniformmanager.h"

namespace QMGE_Core
{

QMGE_GLUniformManager::~QMGE_GLUniformManager()
{

}

QMGE_GLUniformManager * QMGE_GLUniformManager::getInstance()
{
    static QMGE_GLUniformManager m_instance;
    return &m_instance;
}

bool QMGE_GLUniformManager::registerHostUniform(QString name,QMGE_UniformType type,void * pointer)
{
    bool ret = false;
    QMutexLocker locker(&m_uniformsWriteLock);

    auto item = m_uniforms.find(name);
    if(item != m_uniforms.end())
    {
        //already has same key.Check values.
        if(item.value().type == type && item.value().dataPointer == pointer)
        {
            qWarning()<<"Uniform already registered.";
        }
        else
        {
            qWarning("Uniform name conflict detected.Not registering.");
        }
    }
    else
    {
        //item not found.Register a new one.
        m_uniforms.insert(name,QMGE_HostUniform(type,pointer));;
        ret = true;
    }
    return ret;
}

bool QMGE_GLUniformManager::unregisterHostUniform(QString name)
{
    bool ret = false;
    QMutexLocker locker(&m_uniformsWriteLock);

    auto item = m_uniforms.find(name);
    if(item != m_uniforms.end())
    {
        //found
        //check reference count
        if(item.value().refCount != 0)
        {
            qWarning("Can not unregister uniform that holds references.");
        }
        else
        {
            m_uniforms.erase(item);
            ret = true;
        }
    }
    else
    {
        qWarning("Uniform not found.Can not unregister.");
    }

    return ret;
}

bool QMGE_GLUniformManager::bindShaderUniform(QMGE_ShaderUniform &uniform)
{
    bool ret = false;
    QMutexLocker locker(&m_uniformsWriteLock);

    auto item = m_uniforms.find(uniform.name);
    if(item != m_uniforms.end())
    {
        //found
        //link pointer
        uniform.data = item.value().dataPointer;
        //increase reference count
        ++(item.value().refCount);
        ret = true;
    }
    else
    {
        qWarning("Uniform not found in uniformManager.Can not bind.");
    }

    return ret;
}

void QMGE_GLUniformManager::unbindShaderUniform(QMGE_ShaderUniform &uniform)
{
    QMutexLocker locker(&m_uniformsWriteLock);

    auto item = m_uniforms.find(uniform.name);
    if(item != m_uniforms.end())
    {
        //found
        //unlink pointer
        uniform.data = nullptr;
        //remove reference count
        --(item.value().refCount);
    }
    else
    {
        qWarning("Uniform not found in uniformManager.Can not unbind.");
    }
}

QMGE_HostUniform QMGE_GLUniformManager::getUniform(QString name)
{
    QMGE_HostUniform ret;
    QMutexLocker locker(&m_uniformsWriteLock);

    auto item = m_uniforms.find(name);
    if(item != m_uniforms.end())
    {
        //found
        ret = item.value();
    }
    else
    {
        qWarning("Can not get uniform.Uniform not found.");
    }

    return ret;
}


QMGE_GLUniformManager::QMGE_GLUniformManager()
{

}


}
//namespace QMGE_Core
