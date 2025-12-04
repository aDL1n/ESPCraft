#include <cstdint>
#include <ArxContainer.h>

#include <sgl.h>

#include "blocks.h"

namespace world
{   
    class Chunk
    {
    public:
        static const int SIZE = 16;
        sgl::IVec3 position;

        Chunk(sgl::IVec3 pos);
        ~Chunk();

        void setBlock(sgl::IVec3 position, uint8_t type);
        uint8_t getBlock(sgl::IVec3 position) const;

        void rebuildMesh();

        const sgl::Mesh& getMesh() { return mesh; }
    private:
        uint8_t* blocks;
        sgl::Mesh mesh;
    };
}