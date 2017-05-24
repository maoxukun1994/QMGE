#ifndef CHUNKUPDATER_H
#define CHUNKUPDATER_H

#include "massiveground/chunk.h"
#include <QSharedPointer>
#include <QMutexLocker>
#include <QMutex>
#include <QObject>

class ChunkUpdater : public QObject
{

    Q_OBJECT

public:

    ChunkUpdater();

public slots:

    void updateChunkBatch(Chunk * chunk);

};

#endif // CHUNKUPDATER_H
