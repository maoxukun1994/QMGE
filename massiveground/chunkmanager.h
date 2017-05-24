#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

#include <QThread>
#include <QObject>
#include <QOpenGLTexture>
#include "massiveground/chunk.h"
#include "massiveground/chunkupdater.h"
#include "src/qmge_glshaderprogram.h"
#include "src/qmge_uniformprovider.h"

class ChunkManager : public QObject,public QMGE_Core::QMGE_UniformProvider
{
    Q_OBJECT

public:

    ChunkManager();

    ~ChunkManager();

    void loadMap(QString heightMapFileName);

    void update(QVector3D currentPos);

    void move(QVector3D pos);

protected:

    uint calculateLod(QVector3D pos1,QVector3D pos2);

    void registerUniforms() Q_DECL_OVERRIDE;

signals:

    void chunkNeedUpdate(Chunk *);

private:

    int m_chunkViewDistance;
    float m_imgMapScaleFactor;
    float m_chunkSize;

    QThread m_updateThread;
    QScopedPointer<ChunkUpdater> m_chunkUpdater;
    QSizeF m_mapSize;
    QVector3D m_viewPos;
    QSharedPointer<QOpenGLTexture> m_mapTexture;
    QSharedPointer<QMGE_Core::QMGE_GLShaderProgram> m_shader;
    QHash<QString,QSharedPointer<Chunk>> m_chunks;

    //shaderuniforms
    float * maxWidth;
    float * maxHeight;
    float * heightScale;
    int * tex;
    int * ctex;
    float * time;
    QMatrix4x4 * mMatrix;
};

#endif // CHUNKMANAGER_H
