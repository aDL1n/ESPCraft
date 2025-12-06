#include "world.h"

namespace world
{
    World::World()
    {
        chunks.fill(nullptr);
    }

    void World::generate()
    {
        for (int16_t y = 0; y < HEIGHT; y++)
            for (int16_t x = -(SIZE / 2); x < SIZE / 2; x++)
            // for (int16_t z = -1; z < 2; z++)
            {
                Chunk *chunk = new Chunk(sgl::IVec3(x, 0, y));
                for (uint8_t x = 0; x < Chunk::SIZE; x++)
                    for (uint8_t z = 0; z < Chunk::SIZE; z++)
                        for (uint8_t y = 0; y < Chunk::SIZE; y++)
                            chunk->setBlock(sgl::IVec3(x, y, z), BlockType::GRASS);

                chunk->rebuildMesh();

                chunks[(x + (World::SIZE / 2)) + (y * World::SIZE)] = chunk;
            }
    }

    void World::setChunk(Chunk *chunk, sgl::IVec3 position)
    {
        int16_t offset = SIZE / 2;

        if (position.x < -offset || position.x >= offset)
            return;
        if (position.y >= HEIGHT)
            return;

        int index = position.x + offset + (position.z * SIZE);

        if (chunks[index] != nullptr)
            delete chunks[index];

        chunks[index] = chunk;

        if (chunk != nullptr)
            chunk->position = position;
    }

    Chunk *World::getChunk(sgl::IVec3 chunk_position) const
    {
        int16_t offset = SIZE / 2;

        if (chunk_position.x < -offset || chunk_position.x >= offset)
            return nullptr;

        if (chunk_position.z < 0 || chunk_position.z >= HEIGHT)
            return nullptr;

        return chunks[chunk_position.x + offset + (chunk_position.z * SIZE)];
    }

    std::array<Chunk *, World::SIZE * World::HEIGHT> World::getChunks()
    {
        return this->chunks;
    }

    uint8_t World::getBlock(sgl::IVec3 position) const
    {
        int32_t cx = position.x >> 4;
        int32_t cy = position.y >> 4;
        int32_t cz = position.z >> 4;

        Chunk *chunk = this->getChunk(sgl::IVec3(cx, cy, cz));
        if (chunk == nullptr)
            return 0;

        return chunk->getBlock(sgl::IVec3(
            position.x & 15,
            position.y & 15,
            position.z & 15));
    }

    void World::setBlock(uint8_t block, sgl::IVec3 position)
    {
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
