#include "chunk.h"

Chunk::Chunk(QPointF start, float size)
{
    m_startPos = start;
    m_chunkSize = size;
    m_lodLevel = 0;
}

void Chunk::draw()
{
    if(!m_currentBatch.isNull())
    {
        m_currentBatch->draw();
    }
    else
    {
        rebuildBatch();
    }
}

void Chunk::changeLodTo(uint targetLod)
{
    if(m_lodLevel == targetLod) return;

    if(targetLod > CHUNK_MAX_LOD_LEVEL) return;

    m_lodLevel = targetLod;

    rebuildBatch();
}

QVector2D Chunk::getStartPos()
{
    QVector2D ret(m_startPos.x(),m_startPos.y());
    return ret;
}

QVector3D Chunk::getCenterPos()
{
    QVector3D ret(m_startPos.x()+m_chunkSize/2,m_startPos.y()+m_chunkSize/2,0.0f);
    return ret;
}

float Chunk::getSize()
{
    return m_chunkSize;
}


void Chunk::rebuildBatch()
{
    int uCount = (int)qPow(2.0f,(float)(CHUNK_MAX_LOD_LEVEL-m_lodLevel));
    float delta = 1.0f / (float)uCount;
    if(m_chunkSize<delta)
    {
        //reset null
        m_currentBatch.reset();
        return;
    }

    float hstart = m_startPos.x();
    float hend = m_startPos.x() + m_chunkSize;
    float vstart = m_startPos.y()+m_chunkSize;
    float vend = m_startPos.y();

    QScopedPointer<QVector<float>> vec;
    vec.reset(new QVector<float>);

    float h;
    float v;
    QPointF vert[3];
    int vertindex;

    for(v = vstart-delta;v > vend;v -= delta)
    {
        vert[0].setX(hstart);
        vert[0].setY(v+delta);
        vert[1].setX(hstart);
        vert[1].setY(v);
        vertindex = 2;
        for(h = hstart+delta;h < hend;++vertindex)
        {
            int n,n_1,n_2;
            n = vertindex%3;
            n_1 = (n-1<0)?n+2:n-1;
            n_2 = (n-2<0)?n+1:n-2;
            if(vertindex%2)
            {
                vert[n].setX(h);
                vert[n].setY(v);

                vec->push_back(vert[n_1].x());
                vec->push_back(vert[n_1].y());
                vec->push_back(vert[n_2].x());
                vec->push_back(vert[n_2].y());
                vec->push_back(vert[n].x());
                vec->push_back(vert[n].y());

                h += delta;
            }
            else
            {
                vert[n].setX(h);
                vert[n].setY(v+delta);

                vec->push_back(vert[n_2].x());
                vec->push_back(vert[n_2].y());
                vec->push_back(vert[n_1].x());
                vec->push_back(vert[n_1].y());
                vec->push_back(vert[n].x());
                vec->push_back(vert[n].y());
            }
        }
    }

    m_currentBatch.reset(new QMGE_Core::QMGE_GLBatch());
    m_currentBatch->enableBatchVertexAttrib(QMGE_Core::VA_TUV_0);
    m_currentBatch->setVertexData((GLfloat *)vec->data(),(GLint)vec->count()/2,QMGE_Core::VA_TUV_0);
}
