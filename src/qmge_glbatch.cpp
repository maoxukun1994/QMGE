#include "qmge_glbatch.h"

namespace QMGE_Core
{

QMGE_GLBatch::QMGE_GLBatch() : QOpenGLExtraFunctions()
{
    m_primitiveType = GL_TRIANGLES;
    //set vertex count to 0
    m_vertexCount = 0;

    //status as 0(none)
    m_vertexAttrStatus = 0;
    //all expected status as 0(none)
    m_vertexAttrExpectedStatus = 0;

    m_indexDrawing = false;

    m_indexCount = 0;

    //ensure gl functions are loaded before any gl call starts
    initializeOpenGLFunctions();

    //create vao for this batch
    if(!m_vao.create())
    {
        qFatal("Can not create VAO in GLBatch.May be OpenGL(context or functions) was not initialized ?");
    }
}

QMGE_GLBatch::~QMGE_GLBatch()
{
    //delete vbos
    for(int i=0;i<(int)VA_LAST;++i)
    {
        disableBatchVertexAttrib((QMGE_VAttributes)i,true);
    }

    //finally release vao,vao must be the last one to release
    if(m_vao.isCreated())
    {
        m_vao.release();
        m_vao.destroy();
    }
}

void QMGE_GLBatch::setPrimitiveType(GLenum primType)
{
    m_primitiveType = primType;
}

void QMGE_GLBatch::setVertexData(GLfloat * data, GLint vertexCount, QMGE_VAttributes type, QOpenGLBuffer::UsagePattern usage)
{
    //check attribute type
    checkType(type);

    if(!isAttrEnabled(type))
    {
        return;
    }
    //first unset data
    unsetVertexData(type);

    //set
    m_vbo[(int)type].setUsagePattern(usage);
    if(m_vbo[(int)type].create())
    {
        if(m_vbo[(int)type].bind())
        {
            switch (type)
            {
            case VA_POSITION:
            case VA_NORMAL:
                m_vbo[(int)type].allocate(data,sizeof(GLfloat) * vertexCount * 3);
                break;
            case VA_COLOR:
                m_vbo[(int)type].allocate(data,sizeof(GLfloat) * vertexCount * 4);
                break;
            default:
                m_vbo[(int)type].allocate(data,sizeof(GLfloat) * vertexCount * 2);
                break;
            }
            //release
            m_vbo[(int)type].release();
        }
        else
        {
            qFatal("VBO bind failure.");
        }
    }
    else
    {
        qFatal("VBO creation failure.");
    }

    //set vertex count,always the minimum
    if(m_vertexCount == 0)
    {
        m_vertexCount = vertexCount;
    }
    else
    {
        if(m_vertexCount > vertexCount)
        {
            m_vertexCount = vertexCount;
        }
    }

    //set status
    setAttrStatusBit(type);

    //update vao
    updateVAO();
}

void QMGE_GLBatch::setIndexData(GLuint * data,GLuint indexCount)
{
    m_ebo.reset(new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer));
    if(m_ebo->create() && m_ebo->bind())
    {
        m_ebo->allocate(data,sizeof(GLuint)*indexCount);
        m_ebo->release();
        m_indexCount = indexCount;
        m_indexDrawing = true;
    }
    else
    {
        qFatal("Can not set index buffer data!");
    }
}

//if attribute set to VA_LAST,will disable all attributes
void QMGE_GLBatch::enableBatchVertexAttrib(QMGE_VAttributes attribute)
{
    checkType(attribute);
    //set expected status
    setAttrExpectStatusBit(attribute);
    //do not forget to update VAO
    updateVAO();
}

void QMGE_GLBatch::disableBatchVertexAttrib(QMGE_VAttributes attribute,bool unsetData)
{
    checkType(attribute);
    //unset expected status
    unsetAttrExpectStatusBit(attribute);

    //if to unset data
    if(unsetData)
    {
        //then unset the disabled vbo
        unsetVertexData(attribute);
    }
    //do not forget to update VAO
    updateVAO();
}

void QMGE_GLBatch::draw()
{
    m_vao.bind();
    if(m_indexDrawing)
    {
        m_ebo->bind();
        glDrawElements(m_primitiveType,m_indexCount,GL_UNSIGNED_INT,0);
        m_ebo->release();
    }
    else
    {
        glDrawArrays(m_primitiveType,0,m_vertexCount);
    }
    m_vao.release();
}


bool QMGE_GLBatch::isAttrEnabled(QMGE_VAttributes attribute)
{
    unsigned int result = 0;
    result = result | (1<<(int)attribute);

    if(result & m_vertexAttrExpectedStatus)
    {
        return true;
    }
    return false;
}

bool QMGE_GLBatch::isAttrDataSet(QMGE_VAttributes attribute)
{
    unsigned int result = 0;
    result = result | (1<<(int)attribute);

    if(result & m_vertexAttrStatus)
    {
        return true;
    }
    return false;
}

void QMGE_GLBatch::unsetVertexData(QMGE_VAttributes type)
{
    checkType(type);
    if(m_vbo[(int)type].isCreated())
    {
        if(m_vao.isCreated())
        {
            //unbind vao
            m_vao.release();
            //unbind buffer
            m_vbo[(int)type].release();
            //delete buffer
            m_vbo[(int)type].destroy();
            //unset m_vertexAttrStatus
            unsetAttrStatusBit(type);
            //set vertexcount
            m_vertexCount = 0;
        }
    }
    return;
}

void QMGE_GLBatch::setAttrStatusBit(QMGE_VAttributes attribute)
{
    m_vertexAttrStatus = m_vertexAttrStatus | (1<<(int)attribute);
}

void QMGE_GLBatch::unsetAttrStatusBit(QMGE_VAttributes attribute)
{
    unsigned int temp = ~(1<<(int)attribute);
    m_vertexAttrStatus = m_vertexAttrStatus & temp;
}

void QMGE_GLBatch::setAttrExpectStatusBit(QMGE_VAttributes attribute)
{
    m_vertexAttrExpectedStatus = m_vertexAttrExpectedStatus | (1<<(int)attribute);
}

void QMGE_GLBatch::unsetAttrExpectStatusBit(QMGE_VAttributes attribute)
{
    unsigned int temp = ~(1<<(int)attribute);
    m_vertexAttrExpectedStatus = m_vertexAttrExpectedStatus & temp;
}

void QMGE_GLBatch::updateVAO()
{
    //delete if vao exists
    if(m_vao.isCreated())
    {
        m_vao.release();
        m_vao.destroy();
    }

    //create new vao
    if(!m_vao.create())
    {
        qFatal("Can not create VAO in GLBatch.May be OpenGL(context or functions) was not initialized ?");
    }
    m_vao.bind();

    //starting collect vertex attribute infomation
    for(int i=0;i<VA_LAST;++i)
    {
        //skip unabled attributes and skip attributes which haven't been set data
        if(!isAttrEnabled((QMGE_VAttributes)i) || !isAttrDataSet((QMGE_VAttributes)i)) continue;


        if(m_vbo[i].bind())
        {
            switch (i)
            {
            case (int)VA_POSITION:
            case (int)VA_NORMAL:
                glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
                break;
            case (int)VA_COLOR:
                glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
                break;
            case (int)VA_TUV_0:
            case (int)VA_TUV_1:
            case (int)VA_TUV_2:
            case (int)VA_TUV_3:
                glVertexAttribPointer(i, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
                break;
            default:
                break;
            }
            glEnableVertexAttribArray(i);
        }
        else
        {
            qFatal("Can not bind VBO to target.May be platform not support ?");
        }

    }
    //unbind vao
    m_vao.release();
    //release vbos
    glBindBuffer(GL_ARRAY_BUFFER,0);
    return;
}

void QMGE_GLBatch::checkType(QMGE_VAttributes type)
{
    if((int)type >= 0 && (int)type < VA_LAST)
    {
        return;
    }
    else
    {
        qFatal("Invalid vertex attribute type.Check function parameters.");
    }
}










}
//namespace QMGE_Core
