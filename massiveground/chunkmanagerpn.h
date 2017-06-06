#ifndef CHUNKMANAGERPN_H
#define CHUNKMANAGERPN_H

#include <QSharedPointer>
#include <QMatrix4x4>
#include <QVector3D>
#include <QPointF>
#include "src/qmge_glbatch.h"
#include "src/qmge_glshaderprogram.h"
#include "src/qmge_uniformprovider.h"

typedef struct chunkpn
{
    QPointF startPos;
    QMatrix4x4 trans;
    float size;
    QString id;
    QPoint sector;
    chunkpn(float startx,float starty,float csize)
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
ChunkPN;

class ChunkManagerPN : public QMGE_Core::QMGE_UniformProvider
{

public:

    ChunkManagerPN();

    void load();
    void update(QVector3D currentPos);
    void move(QVector3D pos);

protected:

    void registerUniforms() Q_DECL_OVERRIDE;
    void genBaseBatch(int baseDetailLevel);

private:

    QSharedPointer<QMGE_Core::QMGE_GLBatch> m_baseBatch;
    QHash<QString,ChunkPN> m_chunks;
    QVector3D m_viewPos;
    QSharedPointer<QMGE_Core::QMGE_GLShaderProgram> m_shader;

    float m_chunkSize;
    int m_chunkViewDistance;

    //uniforms
    QMatrix4x4 * mMatrix;
    QVector3D * viewPos;
    float * heightScale;
    float * nScale;
};


#endif // CHUNKMANAGERPN_H
