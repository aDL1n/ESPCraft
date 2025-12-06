#include "world.h"

namespace world
{
    void World::generate()
    {
        for (int16_t x = -1; x < 2; x++)
        // for (int16_t z = -1; z < 2; z++)
        {
            Chunk *chunk = new Chunk(sgl::IVec3(x, 0, 0));
            for (uint8_t x = 0; x < Chunk::SIZE; x++)
                for (uint8_t z = 0; z < Chunk::SIZE; z++)
                    for (uint8_t y = 0; y < Chunk::SIZE; y++)
                        chunk->setBlock(sgl::IVec3(x, y, z), 1);

            chunk->rebuildMesh();
            chunks.push_back(chunk);
        }
    }

    void World::setChunk(Chunk chunk, sgl::IVec3 position)
    {
        // TODO
    }

    Chunk *World::getChunk(sgl::IVec3 chunk_position) const
    {
        if (chunks.back()->position == chunk_position)
            return chunks.back();

        for (Chunk *chunk : chunks)
            if (chunk->position == chunk_position)
                return chunk;
        return nullptr;
    }

    std::vector<Chunk *> &World::getChunks()
    {
        return this->chunks;
    }

    uint8_t World::getBlock(sgl::IVec3 position) const
    {
        int32_t cx = std::round(position.x / Chunk::SIZE);
        int32_t cy = std::round(position.y / Chunk::SIZE);
        int32_t cz = std::round(position.z / Chunk::SIZE);

        Chunk *chunk = this->getChunk(sgl::IVec3(cx, cy, cz));
        if (chunk == nullptr)
            return BlockType::AIR;

        return chunk->getBlock(sgl::IVec3(
            position.x & 15,
            position.y & 15,
            position.z & 15));
    }

    void World::setBlock(uint8_t block, sgl::IVec3 position)
    {
        // int32_t cx = position.x / Chunk::SIZE;
        // int32_t cy = position.y / Chunk::SIZE;
        // int32_t cz = position.z / Chunk::SIZE;

        int32_t cx = position.x >> 4;
        int32_t cy = position.y >> 4;
        int32_t cz = position.z >> 4;

        Chunk *chunk = this->getChunk(sgl::IVec3(cx, cy, cz));
        if (chunk == nullptr)
            return;

        chunk->setBlock(sgl::IVec3(
                            position.x & 15,
                            position.y & 15,
                            position.z & 15),
                        block);

        chunk->rebuildMesh();
    }
}
