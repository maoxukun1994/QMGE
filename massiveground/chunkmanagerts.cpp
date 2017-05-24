#include "chunkmanagerts.h"

ChunkManagerTS::ChunkManagerTS()
{
    m_chunkSize = 16.0f;

    m_imgMapScaleFactor = 0.10;

    m_chunkViewDistance = 10;
}


void ChunkManagerTS::loadMap(QString heightMapFileName)
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
    m_shader->addShaderFromSourceFile(QOpenGLShader::Vertex,":/shaders/chunkts.vs");
    m_shader->addShaderFromSourceFile(QOpenGLShader::Fragment,":/shaders/chunkts.fs");
    //m_shader->addShaderFromSourceFile(QOpenGLShader::Geometry,":/shaders/chunkts.gs");
    m_shader->addShaderFromSourceFile(QOpenGLShader::TessellationControl,":/shaders/chunkts.tcs");
    m_shader->addShaderFromSourceFile(QOpenGLShader::TessellationEvaluation,":/shaders/chunkts.tes");
    m_shader->setShaderConfigFile(":/shaders/chunkts.config");
    m_shader->linkProgram();
}

void ChunkManagerTS::update(QVector3D currentPos)
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
        QPoint actualSector = it->sector;
        if(actualSector.x()<startSector.x() || actualSector.x()>endSector.x() || actualSector.y()<startSector.y() || actualSector.y()>endSector.y())
        {
            //remove
            it = m_chunks.erase(it);
        }
        else
        {
            ++it;
        }
    }
    //insert new
    for(int i = startSector.y();i <= endSector.y();++i)
    {
        for(int j = startSector.x();j <= endSector.x();++j)
        {
            if(i<0 || i>maxSector.y() || j<0 || j>maxSector.x()) continue;
            QString key = ChunkTS::genKey(j,i);
            //already have
            if(m_chunks.find(key) != m_chunks.end()) continue;
            else
            {
                ChunkTS newChunk(m_chunkSize*j,m_chunkSize*i,m_chunkSize);
                m_chunks.insert(key,newChunk);
            }
        }
    }
}

void ChunkManagerTS::move(QVector3D pos)
{
    if(pos.distanceToPoint(m_viewPos) > m_chunkSize/3)
    {
        update(pos);
    }

    m_mapTexture->bind();
    m_shader->bind();
    *viewPos = pos;
    m_shader->update_frame();
    for(auto p : m_chunks)
    {
        //set chunk dependent uniforms
        //..
        //..

        *mMatrix = p.trans;
        m_shader->update_once();
        if(!m_baseBatch.isNull())
        {
            m_baseBatch->draw();
        }
        else
        {
            genBaseBatch(0);
        }
    }

}


void ChunkManagerTS::registerUniforms()
{
    QMGE_Core::QMGE_GLUniformManager::getInstance()->registerUniform("maxWidth",(float)m_mapSize.width(),maxWidth);
    QMGE_Core::QMGE_GLUniformManager::getInstance()->registerUniform("maxHeight",(float)m_mapSize.height(),maxHeight);
    QMGE_Core::QMGE_GLUniformManager::getInstance()->registerUniform("heightScale",float(m_chunkSize),heightScale);
    QMGE_Core::QMGE_GLUniformManager::getInstance()->registerUniform("tex",int(0),tex);
    QMGE_Core::QMGE_GLUniformManager::getInstance()->registerUniform("mMatrix",QMatrix4x4(),mMatrix);
    QMGE_Core::QMGE_GLUniformManager::getInstance()->registerUniform("viewPos",QVector3D(),viewPos);
}

void ChunkManagerTS::genBaseBatch(int baseDetailLevel)
{

    m_baseBatch.reset(new QMGE_Core::QMGE_GLBatch());

    int uCount = (int)qPow(2.0f,(float)baseDetailLevel);
    float delta = 1.0f / (float)uCount;
    if(m_chunkSize<delta)
    {
        //reset null
        return;
    }

    QScopedPointer<QVector<float>> points;
    QScopedPointer<QVector<GLuint>> indices;
    points.reset(new QVector<float>);
    indices.reset(new QVector<GLuint>);

    float hstart = 0.0f;
    float hend = m_chunkSize;
    float vstart = m_chunkSize;
    float vend = 0.0f;
    float h;
    float v;
    GLuint colSize = 0;
    //prepare points
    for(v=vstart;v>=vend;v-=delta)
    {
        for(h=hstart;h<=hend;h+=delta)
        {
            points->push_back(h);
            points->push_back(v);            
        }
        colSize++;
    }
    //prepare indices
    //quad
    /*
    for(GLuint i=0;i<colSize-1;++i)
    {
        for(GLuint j=0;j<colSize-1;++j)
        {
            indices->push_back(i*colSize + j);
            indices->push_back((i+1)*colSize + j);
            indices->push_back((i+1)*colSize + (j+1));
            indices->push_back(i*colSize + (j+1));
        }
    }
    */
    GLuint verts[3];
    //triangles
    int count = 0;
    for(GLuint i=0;i<colSize-1;++i)
    {
        verts[0]=(i+1)*colSize;
        verts[1]=(i)*colSize;
        count = 2;
        for(GLuint j=0;j<colSize-1;++count)
        {
            int n,n_1,n_2;
            n = count%3;
            n_1 = (n-1<0)?n+2:n-1;
            n_2 = (n-2<0)?n+1:n-2;
            if(count%2)
            {
                verts[n] = i*colSize + (j+1);
                indices->push_back(verts[n_1]);
                indices->push_back(verts[n_2]);
                indices->push_back(verts[n]);
                ++j;
            }
            else
            {
                verts[n] = (i+1)*colSize +(j+1);
                indices->push_back(verts[n_2]);
                indices->push_back(verts[n_1]);
                indices->push_back(verts[n]);
            }
        }
    }

    m_baseBatch->enableBatchVertexAttrib(QMGE_Core::VA_TUV_0);
    m_baseBatch->setVertexData((GLfloat*)points->data(),points->count()/2,QMGE_Core::VA_TUV_0);
    m_baseBatch->setIndexData(indices->data(),indices->count());
    m_baseBatch->setPrimitiveType(GL_PATCHES);

}
