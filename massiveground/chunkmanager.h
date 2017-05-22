#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H


#include <QHash>
#include <QImage>
#include "massiveground/chunk.h"
#include "src/qmge_glshaderprogram.h"

class ChunkManager
{

public:

    ChunkManager();

    void loadMap(QString heightMapFileName);

    void update(QVector3D currentPos);

    void move(QVector3D pos);

protected:

    uint calculateLod(QVector3D pos1,QVector3D pos2);

private:

    int m_chunkViewDistance;
    float m_imgMapScaleFactor;
    float m_chunkSize;

    QSizeF m_mapSize;

    QVector3D m_viewPos;

    QSharedPointer<QOpenGLTexture> m_mapTexture;

    QSharedPointer<QMGE_Core::QMGE_GLShaderProgram> m_shader;

    QHash<QString,Chunk *> m_chunks;

    //shaderuniforms
    float * maxWidth;
    float * maxHeight;
    float * heightScale;
    int * tex;
    int * ctex;
    QMatrix4x4 * mMatrix;

    //temp
    //QMGE_Core::QMGE_GLBatch * testBatch;
    //Chunk * testChunk;
};

#endif // CHUNKMANAGER_H
