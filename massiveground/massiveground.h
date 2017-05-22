#ifndef MASSIVEGROUND_H
#define MASSIVEGROUND_H

#include <QImage>
#include <QPoint>
#include <QVector>
#include <QVector3D>
#include <QOpenGLTexture>
#include <QSharedPointer>

#include "src/qmge_glbatch.h"
#include "src/qmge_glshaderprogram.h"

#define MAX_LOD_LEVEL 5

class DataChunk
{

public:

    QPoint m_startPoint;
    QPoint m_size;

    DataChunk(QPoint start, QPoint size,QVector<QVector<QPoint>> * data);

    QMGE_Core::QMGE_GLBatch * getBatchLOD(int lodLevel);

    int getLODLevel(QVector3D viewcenter);

    void initBatches(int maxLodLevel);

protected:

    QMGE_Core::QMGE_GLBatch * genBatch(int precision);

protected:

    QVector<QVector<QPoint>> * m_mapData;

    QVector<QMGE_Core::QMGE_GLBatch *> m_batches;
};



class MassiveGround
{

public:

    MassiveGround();

    //precision minimum as 1,for 1 vertex -- 1 pixel
    void loadFromImage(QString filename, int chunkH, int chunkV);

    void draw(QMGE_Core::QMGE_GLShaderProgram * prog);

    QPoint getImgSize();

    QMatrix4x4 * mMatrix;

protected:

    QVector<DataChunk *> m_chunks;
    QVector<QVector<QPoint>> m_mapData;

    int m_slicesH;
    int m_slicesV;
    int m_imgWidth;
    int m_imgHeight;
    float m_precision;

    QSharedPointer<QOpenGLTexture> groundMap;
};

#endif // MASSIVEGROUND_H
