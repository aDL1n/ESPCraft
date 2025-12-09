#include "world.h"

namespace world
{
    World::World()
    {
        chunks.fill(nullptr);
    }

    void World::generate()
    {
        int16_t offset = SIZE / 2;

        for (int16_t cx = -offset; cx < offset; cx++)
            for (int16_t cz = -offset; cz < offset; cz++)
                for (int16_t cy = 0; cy < HEIGHT; cy++)
                {
                    Chunk *chunk = new Chunk(sgl::IVec3(cx, cy, cz));
                    for (uint8_t x = 0; x < Chunk::SIZE; x++)
                        for (uint8_t z = 0; z < Chunk::SIZE; z++)
                            for (uint8_t y = 0; y < Chunk::SIZE; y++)
                                chunk->setBlock(sgl::IVec3(x, y, z), BlockType::GRASS);

                    chunk->rebuildMesh();

                    chunks[(cx + offset) + SIZE * (cy + HEIGHT * (cz + offset))] = chunk;
                }
    }

    void World::setChunk(Chunk *chunk, sgl::IVec3 position)
    {
        int16_t offset = SIZE / 2;

        if (position.x < -offset || position.x >= offset)
            return;

        if (position.z < -offset || position.z >= offset)
            return;

        if (position.y < 0 || position.y >= HEIGHT)
            return;

        int index = (position.x + offset) + SIZE * (position.y + HEIGHT * (position.z + offset));

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

        if (chunk_position.z < -offset || chunk_position.z >= offset)
            return nullptr;

        if (chunk_position.y < 0 || chunk_position.y >= HEIGHT)
            return nullptr;

        return chunks[(chunk_position.x + offset) + SIZE * (chunk_position.y + HEIGHT * (chunk_position.z + offset))];
    }

    std::array<Chunk *, World::SIZE * World::SIZE * World::HEIGHT> &World::getChunks()
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

    BlockHit World::getBlockAtView(sgl::Vec3 position, sgl::Vec3 direction, uint8_t max_distance)
    {   
        int32_t x = floorf(position.x);
        int32_t y = floorf(position.y);
        int32_t z = floorf(position.z);

        int8_t step_x = (direction.x >= 0) ? 1 : -1;
        int8_t step_y = (direction.y >= 0) ? 1 : -1;
        int8_t step_z = (direction.z >= 0) ? 1 : -1;

        float delta_x = (direction.x == 0.0f) ? std::numeric_limits<float>::max() : std::abs(1.0f / direction.x);
        float delta_y = (direction.y == 0.0f) ? std::numeric_limits<float>::max() : std::abs(1.0f / direction.y);
        float delta_z = (direction.z == 0.0f) ? std::numeric_limits<float>::max() : std::abs(1.0f / direction.z);

        float start_x = (direction.x >= 0) ? (x + 1 - position.x) * delta_x : (x - position.x) * delta_x;
        float start_y = (direction.y >= 0) ? (y + 1 - position.y) * delta_y : (y - position.y) * delta_y;
        float start_z = (direction.z >= 0) ? (z + 1 - position.z) * delta_z : (z - position.z) * delta_z;

        while (start_x < max_distance || start_y < max_distance || start_z < max_distance)
        {   
            uint8_t block_type = this->getBlock(sgl::IVec3(x, y, z));
            if (block_type != BlockType::AIR)
            {
                BlockHit hit;
                hit.hit = true;
                hit.type = block_type;
                hit.position = sgl::IVec3(x, y, z);

                return hit;
            }

            if (start_x < start_y && start_x < start_z)
            {
                x += step_x;
                start_x += delta_x;
            } 
            else if (start_y < start_z)
            {
                y += step_y;
                start_y += delta_y;
            }   
            else
            {
                z += step_z;
                start_z += delta_z;
            }
        }

        return BlockHit({false});
    }
}