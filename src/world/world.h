#pragma once

#include <ArxContainer.h>

#include "chunk.h"
#include "math/ivec3.h"

namespace world
{
    class World
    {
    public:
        static const uint8_t HEIGHT = 1;
        static const uint8_t SIZE = 7; // 7 max = 245kb in ram

        World();

        void generate();

        void setChunk(Chunk *chunk, sgl::IVec3 position);

        Chunk* getChunk(sgl::IVec3 chunk_position) const; 
        std::array<Chunk*,SIZE * SIZE * HEIGHT> &getChunks();

        uint8_t getBlock(sgl::IVec3 position) const;
        void setBlock(uint8_t block, sgl::IVec3 position);
    private:
        std::array<Chunk*, SIZE * SIZE * HEIGHT> chunks;
    };
}