#ifndef CHUNKMANAGERTS_H
#define CHUNKMANAGERTS_H

#include <QSharedPointer>
#include <QMatrix4x4>
#include <QVector3D>
#include <QPointF>
#include <QHash>
#include "src/qmge_glbatch.h"
#include "src/qmge_glshaderprogram.h"
#include "src/qmge_uniformprovider.h"

typedef struct chunkts
{
    QPointF startPos;
    QMatrix4x4 trans;
    float size;
    QString id;
    QPoint sector;
    chunkts(float startx,float starty,float csize)
    {
        startPos.setX(startx);
        startPos.setY(starty);
        trans.setToIdentity();
        trans.translate(startx,starty,0.0f);
        size = csize;
        int sectorx = (int)((startx+1.0f)/size);
        int sectory = (int)((starty+1.0f)/size);
        id = genKey(sectorx,sectory);
        sector.setX(sectorx);
        sector.setY(sectory);
    }

    static QString genKey(int x,int y)
    {
        QString id;
        id.append(QString::number(x));
        id.append("#");
        id.append(QString::number(y));
        return id;
    }
}
ChunkTS;

class ChunkManagerTS : public QMGE_Core::QMGE_UniformProvider
{

public:

    ChunkManagerTS();

    void loadMap(QString heightMapFileName, QString normalMapFilename);

    void update(QVector3D currentPos);

    void move(QVector3D pos);

protected:

    void registerUniforms() Q_DECL_OVERRIDE;

    void genBaseBatch(int baseDetailLevel);

private:

    QSharedPointer<QMGE_Core::QMGE_GLBatch> m_baseBatch;
    QHash<QString,ChunkTS> m_chunks;
    QVector3D m_viewPos;
    QSizeF m_mapSize;
    QSharedPointer<QOpenGLTexture> m_mapTexture;
    QSharedPointer<QOpenGLTexture> m_normalTexture;
    QSharedPointer<QMGE_Core::QMGE_GLShaderProgram> m_shader;

    float m_chunkSize;
    float m_imgMapScaleFactor;
    int m_chunkViewDistance;

    //uniforms
    QMatrix4x4 * mMatrix;
    QVector3D * viewPos;
    float * maxWidth;
    float * maxHeight;
    float * heightScale;
    int * tex;
    int * norm;
};

#endif // CHUNKMANAGERTS_H
