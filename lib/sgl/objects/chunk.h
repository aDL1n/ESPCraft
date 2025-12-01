#include <cstdint>
#include <ArxContainer.h>

#include "math/ivec3.h"

namespace sgl
{   
    struct ChunkFace {
        uint8_t x, y, z;
        uint8_t faceIndex;
        uint8_t color;
        uint8_t w, h;
    };

    class Chunk
    {
    public:
        static const int SIZE = 16;
        IVec3 position;

        Chunk(IVec3 pos);
        ~Chunk();

        void setBlock(IVec3 position, uint8_t type);
        uint8_t getBlock(IVec3 position) const;

        void rebuildMesh();

        void compress();

        const std::vector<ChunkFace>& getMesh() const { return mesh; }
    private:
        uint8_t* blocks;
        std::vector<ChunkFace> mesh;

        bool isSolid(int x, int y, int z) const;
    };
    
}