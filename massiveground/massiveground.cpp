#include "massiveground.h"

DataChunk::DataChunk(QPoint start, QPoint size, QVector<QVector<QPoint>> * data):m_startPoint(start),m_size(size),m_mapData(data)
{

}


QMGE_Core::QMGE_GLBatch * DataChunk::getBatchLOD(int lodLevel)
{
    if(lodLevel<0 || lodLevel>MAX_LOD_LEVEL) return nullptr;
    if(m_batches.count()<lodLevel-1) return nullptr;
    return m_batches.at(lodLevel);
}

int DataChunk::getLODLevel(QVector3D viewcenter)
{
    return 0;
}

void DataChunk::initBatches(int maxLodLevel)
{
    if(maxLodLevel<0 || maxLodLevel > MAX_LOD_LEVEL)
    {
        qFatal("Invalid maxLODLevel");
        return;
    }

    int precision = 1;
    for(int i=0;i<=maxLodLevel;++i)
    {
        auto batch = genBatch(precision);
        m_batches.push_back(batch);
        precision *= 2;
    }

}

QMGE_Core::QMGE_GLBatch * DataChunk::genBatch(int precision)
{
    int endx = m_startPoint.x() + m_size.x();
    int endy = m_startPoint.y() + m_size.y();

    float scale = 1.0f;

    QVector<float> * vert = new QVector<float>;
    QPoint v1,v2,v3,v4;

    for(int i=m_startPoint.y();i<endy;i+=precision)
    {
        for(int j=m_startPoint.x();j<endx;j+=precision)
        {
            if(i+precision>=endy || j+precision >= endx) continue;
            v1 = m_mapData->at(i).at(j) * scale;
            v2 = m_mapData->at(i+precision).at(j) * scale;
            v3 = m_mapData->at(i).at(j+precision) * scale;
            v4 = m_mapData->at(i+precision).at(j+precision) * scale;
            vert->push_back(v1.x());
            vert->push_back(v1.y());
            vert->push_back(0.0f);
            vert->push_back(v2.x());
            vert->push_back(v2.y());
            vert->push_back(0.0f);
            vert->push_back(v3.x());
            vert->push_back(v3.y());
            vert->push_back(0.0f);

            vert->push_back(v3.x());
            vert->push_back(v3.y());
            vert->push_back(0.0f);
            vert->push_back(v2.x());
            vert->push_back(v2.y());
            vert->push_back(0.0f);
            vert->push_back(v4.x());
            vert->push_back(v4.y());
            vert->push_back(0.0f);
        }
    }

    auto batch = new QMGE_Core::QMGE_GLBatch();
    batch->enableBatchVertexAttrib(QMGE_Core::QMGE_VAttributes::VA_POSITION);
    batch->setVertexData(vert->data(),vert->count(),QMGE_Core::QMGE_VAttributes::VA_POSITION);

    return batch;
}

MassiveGround::MassiveGround()
{

}

void MassiveGround::loadFromImage(QString filename, int chunkH, int chunkV)
{
    QImage img(filename);

    if(img.isNull())
    {
        qWarning()<<"Can not load ground image file.";
        return;
    }

    m_imgHeight = img.height();
    m_imgWidth = img.width();

    //set texture
    groundMap.reset(new QOpenGLTexture(img));
    groundMap->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
    groundMap->setMinificationFilter(QOpenGLTexture::Linear);


    //set mapdata
    for(int i=0;i<img.height();i++)
    {
        QVector<QPoint> colData;
        for(int j=0;j<img.width();j++)
        {
            colData.push_back(QPoint(i,j));
        }
        m_mapData.push_back(colData);
    }

    int deltaH = img.width()/chunkH;
    int deltaV = img.height()/chunkV;

    for(int i=0;i<chunkV;++i)
    {
        for(int j=0;j<chunkH;++j)
        {
            auto chunk = new DataChunk(QPoint(deltaH*j,deltaV*i),QPoint(deltaH,deltaV),&m_mapData);
            m_chunks.push_back(chunk);
            chunk->initBatches(MAX_LOD_LEVEL);
        }
    }

}

void MassiveGround::draw(QMGE_Core::QMGE_GLShaderProgram *prog)
{
    for(int i=0;i<m_chunks.count();++i)
    {
        mMatrix->setToIdentity();
        mMatrix->translate(QVector3D(m_chunks.at(i)->m_startPoint.x(),m_chunks.at(i)->m_startPoint.y(),0));
        prog->update_once();
        m_chunks.at(i)->getBatchLOD(2)->draw();
    }
}

QPoint MassiveGround::getImgSize()
{
    return QPoint(m_imgWidth,m_imgHeight);
}
