#include "chunk.h"

Chunk::Chunk(QPointF start, float size)
{
    m_startPos = start;
    m_chunkSize = size;
    m_lodLevel = 0;
    m_dataBuffer.reset(new QVector<float>);
    m_dataBuffer->clear();
    m_batchNeedUpdate = false;
    m_lifeTime = 1.5f;
}

Chunk::~Chunk()
{
    m_dataLocker.unlock();
}

void Chunk::draw()
{
    if(m_batchNeedUpdate)
    {
        rebuildBatch();
    }

    if(!m_currentBatch.isNull())
    {
        m_currentBatch->draw();
        if(m_lifeTime > 0)
        {
            m_lifeTime -= 0.03f;
        }
        else
        {
            m_lifeTime = 0.0f;
        }
    }
}

bool Chunk::changeLodTo(uint targetLod)
{
    if(targetLod > CHUNK_MAX_LOD_LEVEL) targetLod = CHUNK_MAX_LOD_LEVEL;

    if(m_lodLevel == targetLod) return false;

    m_lodLevel = targetLod;

    return true;
}

QVector2D Chunk::getStartPos()
{
    QVector2D ret(m_startPos.x(),m_startPos.y());
    return ret;
}

QVector3D Chunk::getCenterPos()
{
    QVector3D ret(m_startPos.x()+m_chunkSize/2,m_startPos.y()+m_chunkSize/2,m_chunkSize/2);
    return ret;
}

float Chunk::getSize()
{
    return m_chunkSize;
}

uint Chunk::getLod()
{
    return m_lodLevel;
}

void Chunk::calculateBuffer()
{
    if(m_batchNeedUpdate) return;

    QMutexLocker locker(&m_dataLocker);
    m_dataBuffer->clear();

    int uCount = (int)qPow(2.0f,(float)(CHUNK_MAX_LOD_LEVEL-m_lodLevel));
    float delta = 1.0f / (float)uCount;
    if(m_chunkSize<delta)
    {
        //reset null
        return;
    }

    float hstart = m_startPos.x();
    float hend = m_startPos.x() + m_chunkSize;
    float vstart = m_startPos.y()+m_chunkSize;
    float vend = m_startPos.y();

    float h;
    float v;
    QPointF vert[3];
    int vertindex;

    for(v = vstart-delta;v >= vend;v -= delta)
    {
        vert[0].setX(hstart);
        vert[0].setY(v+delta);
        vert[1].setX(hstart);
        vert[1].setY(v);
        vertindex = 2;
        for(h = hstart+delta;h <= hend;++vertindex)
        {
            int n,n_1,n_2;
            n = vertindex%3;
            n_1 = (n-1<0)?n+2:n-1;
            n_2 = (n-2<0)?n+1:n-2;
            if(vertindex%2)
            {
                vert[n].setX(h);
                vert[n].setY(v);

                m_dataBuffer->push_back(vert[n_1].x());
                m_dataBuffer->push_back(vert[n_1].y());
                m_dataBuffer->push_back(vert[n_2].x());
                m_dataBuffer->push_back(vert[n_2].y());
                m_dataBuffer->push_back(vert[n].x());
                m_dataBuffer->push_back(vert[n].y());

                h += delta;
            }
            else
            {
                vert[n].setX(h);
                vert[n].setY(v+delta);

                m_dataBuffer->push_back(vert[n_2].x());
                m_dataBuffer->push_back(vert[n_2].y());
                m_dataBuffer->push_back(vert[n_1].x());
                m_dataBuffer->push_back(vert[n_1].y());
                m_dataBuffer->push_back(vert[n].x());
                m_dataBuffer->push_back(vert[n].y());
            }
        }
    }

    m_batchNeedUpdate = true;
}

void Chunk::rebuildBatch()
{
    if(!m_batchNeedUpdate || m_dataBuffer->isEmpty()) return;
    m_currentBatch.reset(new QMGE_Core::QMGE_GLBatch());
    m_currentBatch->enableBatchVertexAttrib(QMGE_Core::VA_TUV_0);
    m_currentBatch->setVertexData((GLfloat *)m_dataBuffer->data(),(GLint)m_dataBuffer->count()/2,QMGE_Core::VA_TUV_0);
    m_lifeTime = 1.5f;
    m_batchNeedUpdate = false;
}
