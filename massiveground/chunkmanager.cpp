#include "chunkmanager.h"

ChunkManager::ChunkManager()
{
    m_chunkViewDistance = 8;
    m_imgMapScaleFactor = 0.05f;
    //m_imgMapScaleFactor = 0.5f;
    m_chunkSize = 16.0f;

    m_chunkUpdater.reset(new ChunkUpdater());

    connect(this,SIGNAL(chunkNeedUpdate(Chunk*)),m_chunkUpdater.data(),SLOT(updateChunkBatch(Chunk*)));
}

ChunkManager::~ChunkManager()
{
    m_updateThread.quit();
    m_updateThread.wait();
}

void ChunkManager::loadMap(QString heightMapFileName)
{
    QImage img(heightMapFileName);

    if(img.isNull())
    {
        qFatal("Can not load image file.");
        return;
    }
    m_mapSize.setWidth(img.width()*m_imgMapScaleFactor);
    m_mapSize.setHeight(img.height()*m_imgMapScaleFactor);

    //set texture
    m_mapTexture.reset(new QOpenGLTexture(img));
    m_mapTexture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
    m_mapTexture->setMinificationFilter(QOpenGLTexture::Linear);

    //setup uniforms
    registerUniforms();

    //init shader
    m_shader.reset(new QMGE_Core::QMGE_GLShaderProgram());
    m_shader->addShaderFromSourceFile(QOpenGLShader::Vertex,":/massiveground/shaders/chunk/chunk.vs");
    m_shader->addShaderFromSourceFile(QOpenGLShader::Fragment,":/massiveground/shaders/chunk/chunk.fs");
    m_shader->addShaderFromSourceFile(QOpenGLShader::Geometry,":/massiveground/shaders/chunk/chunk.gs");
    m_shader->setShaderConfigFile(":/massiveground/shaders/chunk/chunk.config");
    m_shader->linkProgram();

    m_chunkUpdater->moveToThread(&m_updateThread);
    m_updateThread.start();
}

void ChunkManager::update(QVector3D currentPos)
{
    m_viewPos = currentPos;
    //calculate area
    QPoint viewSector,startSector,endSector,maxSector;
    viewSector.setX((int)(m_viewPos.x()/m_chunkSize));
    viewSector.setY((int)(m_viewPos.y()/m_chunkSize));
    maxSector.setX(m_mapSize.width()/m_chunkSize);
    maxSector.setY(m_mapSize.height()/m_chunkSize);
    startSector.setX(viewSector.x()-m_chunkViewDistance);
    startSector.setY(viewSector.y()-m_chunkViewDistance);
    endSector.setX(viewSector.x()+m_chunkViewDistance);
    endSector.setY(viewSector.y()+m_chunkViewDistance);
    //remove and update old chunks
    auto it = m_chunks.begin();
    while (it != m_chunks.end())
    {
        QPoint actualSector;
        actualSector.setX((it.value()->getStartPos().x()+1)/m_chunkSize);
        actualSector.setY((it.value()->getStartPos().y()+1)/m_chunkSize);
        if(actualSector.x()<startSector.x() || actualSector.x()>endSector.x() || actualSector.y()<startSector.y() || actualSector.y()>endSector.y())
        {
            //remove
            if(!it.value().isNull())
            {
                //get locker
                it.value()->m_dataLocker.lock();
                it.value().reset();
            }
            it = m_chunks.erase(it);            
        }
        else
        {
            if(it.value()->changeLodTo(calculateLod(it.value()->getCenterPos(),m_viewPos)))
            {
                emit chunkNeedUpdate(it.value().data());
            }
            ++it;
        }
    }
    //insert new
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
                auto cp = new Chunk(QPointF(m_chunkSize*j,m_chunkSize*i),m_chunkSize);
                QSharedPointer<Chunk> newChunk;
                newChunk.reset(cp);
                if(newChunk->changeLodTo(calculateLod(newChunk->getCenterPos(),m_viewPos)))
                {
                    emit chunkNeedUpdate(newChunk.data());
                }
                m_chunks.insert(key,newChunk);
            }
        }
    }

}

void ChunkManager::move(QVector3D pos)
{
    if(pos.distanceToPoint(m_viewPos) > m_chunkSize/3)
    {
        update(pos);
    }

    m_mapTexture->bind();
    m_shader->bind();
    m_shader->update_frame();
    for(auto p : m_chunks)
    {
        *time = p->m_lifeTime;
        m_shader->update_once();
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

void ChunkManager::registerUniforms()
{
    QMGE_Core::QMGE_GLUniformManager::getInstance()->registerUniform("maxWidth",(float)m_mapSize.width(),maxWidth);
    QMGE_Core::QMGE_GLUniformManager::getInstance()->registerUniform("maxHeight",(float)m_mapSize.height(),maxHeight);
    QMGE_Core::QMGE_GLUniformManager::getInstance()->registerUniform("heightScale",float(m_chunkSize),heightScale);
    QMGE_Core::QMGE_GLUniformManager::getInstance()->registerUniform("tex",int(0),tex);
    QMGE_Core::QMGE_GLUniformManager::getInstance()->registerUniform("ctex",int(0),ctex);
    QMGE_Core::QMGE_GLUniformManager::getInstance()->registerUniform("mMatrix",QMatrix4x4(),mMatrix);
    QMGE_Core::QMGE_GLUniformManager::getInstance()->registerUniform("time",float(0.0f),time);
}
