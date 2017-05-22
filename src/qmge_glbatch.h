#ifndef QMGE_GLBATCH_H
#define QMGE_GLBATCH_H

#include "qmge_global.h"


namespace QMGE_Core
{

//do not change this unless extreme necessary
//change this also need to change VA_TUV_X definitions
#define QMGE_MAX_TUV_SET_PER_BATCH 4

enum QMGE_VAttributes
{
    VA_POSITION = 0,
    VA_NORMAL, //1
    VA_COLOR, //2
    VA_TUV_0, //3
    VA_TUV_1, //4
    VA_TUV_2, //5
    VA_TUV_3, //6
    //not valid
    VA_LAST //attribute size
};

class QMGE_GLBatch : protected QOpenGLExtraFunctions
{

public:

    QMGE_GLBatch();
    ~QMGE_GLBatch();

    void setPrimitiveType(GLenum primType);
    void setVertexData(GLfloat * data,GLint vertexCount,QMGE_VAttributes type,QOpenGLBuffer::UsagePattern usage=QOpenGLBuffer::StaticDraw);

    void enableBatchVertexAttrib(QMGE_VAttributes attribute);
    void disableBatchVertexAttrib(QMGE_VAttributes attribute,bool unsetData);

    void draw();

protected:

    bool isAttrEnabled(QMGE_VAttributes attribute);
    bool isAttrDataSet(QMGE_VAttributes attribute);

    void unsetVertexData(QMGE_VAttributes type);

    void setAttrStatusBit(QMGE_VAttributes attribute);
    void unsetAttrStatusBit(QMGE_VAttributes attribute);

    void setAttrExpectStatusBit(QMGE_VAttributes attribute);
    void unsetAttrExpectStatusBit(QMGE_VAttributes attribute);

    void updateVAO();

    void checkType(QMGE_VAttributes type);

protected:

    GLenum m_primitiveType;
    //vbos
    QOpenGLBuffer m_vbo[VA_LAST];
    //vao
    QOpenGLVertexArrayObject m_vao;
    //vertex count
    GLint m_vertexCount;
    //batch vertex attribute status
    unsigned int m_vertexAttrStatus;
    unsigned int m_vertexAttrExpectedStatus;

};
//class QMGE_GLBatch


}
//namespace QMGE_Core

#endif // QMGE_GLBATCH_H
