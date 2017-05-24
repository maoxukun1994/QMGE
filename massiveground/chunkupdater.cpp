#include "chunkupdater.h"

ChunkUpdater::ChunkUpdater()
{
}

void ChunkUpdater::updateChunkBatch(Chunk * chunk)
{
    chunk->calculateBuffer();
    return;
}
