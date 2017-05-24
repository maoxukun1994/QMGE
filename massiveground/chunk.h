#ifndef CHUNK_H
#define CHUNK_H

#include <QSize>
#include <QDateTime>
#include <QSizeF>
#include <QPoint>
#include <QPointF>
#include <QVector>
#include <QMutex>
#include <QMutexLocker>
#include <QVector2D>
#include <QVector3D>
#include <QScopedPointer>
#include <QSharedPointer>

#include "src/qmge_glbatch.h"
#include "src/qmge_gluniformmanager.h"

//vertex count in unit length(1.0f) when lodlevel = 0
#define CHUNK_MAX_LOD_LEVEL 5

class Chunk
{

public:

    Chunk(QPointF start, float size);

    ~Chunk();

    void draw();

    bool changeLodTo(uint targetLod);

    QVector2D getStartPos();

    QVector3D getCenterPos();

    float getSize();

    uint getLod();

    void calculateBuffer();

protected:

    void rebuildBatch();

public:

    //used in geomotry shader for animation
    float m_lifeTime;

    QMutex m_dataLocker;

private:

    QSharedPointer<QMGE_Core::QMGE_GLBatch> m_currentBatch;

    volatile bool m_batchNeedUpdate;

    QScopedPointer<QVector<float>> m_dataBuffer;

    QPointF m_startPos;

    float m_chunkSize;

    uint m_lodLevel;
};

#endif // CHUNK_H
