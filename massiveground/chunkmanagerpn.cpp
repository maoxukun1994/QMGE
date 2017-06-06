#include "chunkmanagerpn.h"

ChunkManagerPN::ChunkManagerPN()
{
    m_chunkSize = 32.0f;
    m_chunkViewDistance = 12;
}

void ChunkManagerPN::load()
{
    //setup uniforms
    registerUniforms();

    //init shader
    m_shader.reset(new QMGE_Core::QMGE_GLShaderProgram());
    m_shader->addShaderFromSourceFile(QOpenGLShader::Vertex,":/massiveground/shaders/chunkpn/chunkpn.vs");
    m_shader->addShaderFromSourceFile(QOpenGLShader::Fragment,":/massiveground/shaders/chunkpn/chunkpn.fs");
    m_shader->addShaderFromSourceFile(QOpenGLShader::TessellationControl,":/massiveground/shaders/chunkpn/chunkpn.tcs");
    m_shader->addShaderFromSourceFile(QOpenGLShader::TessellationEvaluation,":/massiveground/shaders/chunkpn/chunkpn.tes");
    m_shader->setShaderConfigFile(":/massiveground/shaders/chunkpn/chunkpn.config");
    m_shader->linkProgram();
}

void ChunkManagerPN::update(QVector3D currentPos)
{
    m_viewPos = currentPos;
    //calculate area
    QPoint viewSector,startSector,endSector;
    viewSector.setX((int)(m_viewPos.x()/m_chunkSize));
    viewSector.setY((int)(m_viewPos.y()/m_chunkSize));
    startSector.setX(viewSector.x()-m_chunkViewDistance);
    startSector.setY(viewSector.y()-m_chunkViewDistance);
    endSector.setX(viewSector.x()+m_chunkViewDistance);
    endSector.setY(viewSector.y()+m_chunkViewDistance);
    //remove and update old chunks
    auto it = m_chunks.begin();
    while (it != m_chunks.end())
    {
        QPoint actualSector = it->sector;
        //for "circle" chunk load
        int i2 = qAbs(viewSector.y()-it.value().sector.y());
        int j2 = qAbs(viewSector.x()-it.value().sector.x());
        i2 *= i2;
        j2 *= j2;
        if(actualSector.x()<startSector.x() || actualSector.x()>endSector.x() || actualSector.y()<startSector.y() || actualSector.y()>endSector.y() || qSqrt(i2+j2)>(float)m_chunkViewDistance)
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
            //for "circle" chunk load
            int i2 = qAbs(viewSector.y()-i);
            int j2 = qAbs(viewSector.x()-j);
            i2 *= i2;
            j2 *= j2;
            if(qSqrt(i2+j2) > (float)m_chunkViewDistance) continue;

            QString key = ChunkPN::genKey(j,i);
            //already have
            if(m_chunks.find(key) != m_chunks.end()) continue;
            else
            {
                ChunkPN newChunk(m_chunkSize*j,m_chunkSize*i,m_chunkSize);
                m_chunks.insert(key,newChunk);
            }
        }
    }
}

void ChunkManagerPN::move(QVector3D pos)
{
    if(pos.distanceToPoint(m_viewPos) > m_chunkSize/3)
    {
        update(pos);
    }

    m_shader->bind();
    *viewPos = pos;
    m_shader->update_frame();

    if(m_baseBatch.isNull())
    {
        genBaseBatch(-1);
    }

    for(auto p : m_chunks)
    {
        //set chunk dependent uniforms
        *mMatrix = p.trans;
        m_shader->update_once();

        m_baseBatch->draw();
    }
}

void ChunkManagerPN::registerUniforms()
{
    QMGE_Core::QMGE_GLUniformManager::getInstance()->registerUniform("heightScale",float(8.0f),heightScale);
    QMGE_Core::QMGE_GLUniformManager::getInstance()->registerUniform("mMatrix",QMatrix4x4(),mMatrix);
    QMGE_Core::QMGE_GLUniformManager::getInstance()->registerUniform("viewPos",QVector3D(),viewPos);
    QMGE_Core::QMGE_GLUniformManager::getInstance()->registerUniform("nScale",float(20.0f),nScale);
}

void ChunkManagerPN::genBaseBatch(int baseDetailLevel)
{
    m_baseBatch.reset(new QMGE_Core::QMGE_GLBatch());

    float uCount = qPow(2.0f,(float)baseDetailLevel);
    float delta = 1.0f / uCount;
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
