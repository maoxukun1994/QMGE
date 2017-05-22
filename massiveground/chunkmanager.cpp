#include "chunkmanager.h"

ChunkManager::ChunkManager()
{
    m_chunkViewDistance = 4;
    m_imgMapScaleFactor = 0.05f;
    m_chunkSize = 16.0f;
}

void ChunkManager::loadMap(QString heightMapFileName)
{
    QImage img(heightMapFileName);

    if(img.isNull())
    {
        qFatal("Can not load ground image file.");
        return;
    }
    m_mapSize.setWidth(img.width()*m_imgMapScaleFactor);
    m_mapSize.setHeight(img.height()*m_imgMapScaleFactor);

    //set texture
    m_mapTexture.reset(new QOpenGLTexture(img));
    m_mapTexture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
    m_mapTexture->setMinificationFilter(QOpenGLTexture::Linear);

    //setup uniforms
    //shaderuniforms
    QMGE_Core::QMGE_GLUniformManager::getInstance()->registerUniform("maxWidth",(float)m_mapSize.width(),maxWidth);
    QMGE_Core::QMGE_GLUniformManager::getInstance()->registerUniform("maxHeight",(float)m_mapSize.height(),maxHeight);
    QMGE_Core::QMGE_GLUniformManager::getInstance()->registerUniform("heightScale",float(15.0f),heightScale);
    QMGE_Core::QMGE_GLUniformManager::getInstance()->registerUniform("tex",int(0),tex);
    QMGE_Core::QMGE_GLUniformManager::getInstance()->registerUniform("ctex",int(0),ctex);
    QMGE_Core::QMGE_GLUniformManager::getInstance()->registerUniform("mMatrix",QMatrix4x4(),mMatrix);

    //init shader
    m_shader.reset(new QMGE_Core::QMGE_GLShaderProgram());
    m_shader->addShaderFromSourceFile(QOpenGLShader::Vertex,":/shaders/chunk.vs");
    m_shader->addShaderFromSourceFile(QOpenGLShader::Fragment,":/shaders/chunk.fs");
    m_shader->setShaderConfigFile(":/shaders/chunk.config");
    m_shader->linkProgram();

    /*
    //setup test batch
    GLfloat tuvs[] =
    {
        0.0f,28.0f,
        0.0f,0.0f,
        57.0f,0.0f,
        57.0f,28.0f,
        0.0f,28.0f,
        57.0f,0.0f
    };
    testBatch = new QMGE_Core::QMGE_GLBatch();
    testBatch->enableBatchVertexAttrib(QMGE_Core::VA_TUV_0);
    testBatch->setVertexData(tuvs,6,QMGE_Core::VA_TUV_0);
    testChunk = new Chunk(QPointF(0,0),m_chunkSize);
    testChunk->changeLodTo(0);
    */
}

void ChunkManager::update(QVector3D currentPos)
{
    m_viewPos = currentPos;
    //remove old chunks
    auto it = m_chunks.begin();
    while (it != m_chunks.end())
    {
        if( (it.value()->getStartPos()+QVector2D(m_chunkSize/2,m_chunkSize/2)).distanceToPoint(QVector2D(m_viewPos)) > m_chunkViewDistance * m_chunkSize)
        {
            it = m_chunks.erase(it);
        }
        else
        {
            it.value()->changeLodTo(calculateLod(it.value()->getCenterPos(),m_viewPos));
            ++it;
        }
    }
    //insert new
    QPoint viewSector,startSector,endSector,maxSector;
    viewSector.setX((int)(m_viewPos.x()/m_chunkSize));
    viewSector.setY((int)(m_viewPos.y()/m_chunkSize));
    maxSector.setX(m_mapSize.width()/m_chunkSize);
    maxSector.setY(m_mapSize.height()/m_chunkSize);
    startSector.setX(viewSector.x()-m_chunkViewDistance);
    startSector.setY(viewSector.y()-m_chunkViewDistance);
    endSector.setX(viewSector.x()+m_chunkViewDistance);
    endSector.setY(viewSector.y()+m_chunkViewDistance);
    for(int i = startSector.y();i <= endSector.y();++i)
    {
        for(int j = startSector.x();j <= endSector.x();++j)
        {
            if(i<0 || i>maxSector.y() || j<0 || j>maxSector.x()) continue;
            QString key;
            key.append(QString::number(j));
            key.append("#");
            key.append(QString::number(i));

            //already have
            if(m_chunks.find(key) != m_chunks.end()) continue;
            else
            {
                auto newChunk = new Chunk(QPointF(m_chunkSize*j,m_chunkSize*i),m_chunkSize);
                newChunk->changeLodTo(calculateLod(newChunk->getCenterPos(),m_viewPos));
                m_chunks.insert(key,newChunk);
            }
        }
    }

}

void ChunkManager::move(QVector3D pos)
{
    if(pos.distanceToPoint(m_viewPos) > m_chunkSize/5)
    {
        update(pos);
        qDebug()<<"updated";
    }


    m_mapTexture->bind();
    m_shader->bind();
    m_shader->update_frame();
    //testBatch->draw();
    //testChunk->draw();


    for(auto p : m_chunks)
    {
        p->draw();
    }
}

uint ChunkManager::calculateLod(QVector3D pos1,QVector3D pos2)
{
    float distance = pos1.distanceToPoint(pos2);
    uint lod = distance / (m_chunkSize/3);
    if(lod > CHUNK_MAX_LOD_LEVEL) lod = CHUNK_MAX_LOD_LEVEL;
    return lod;
}
