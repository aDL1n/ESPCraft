#pragma once

#include <ArxContainer.h>

#include "chunk.h"
#include "math/ivec3.h"

namespace world
{
    class World
    {
    public:
        void generate();

        void setChunk(Chunk chunk, sgl::IVec3 position);

        Chunk* getChunk(sgl::IVec3 chunk_position) const; 
        std::vector<Chunk*> &getChunks();

        uint8_t getBlock(sgl::IVec3 position) const;
        void setBlock(uint8_t block, sgl::IVec3 position);
    private:
        std::vector<Chunk*> chunks;
    };
}