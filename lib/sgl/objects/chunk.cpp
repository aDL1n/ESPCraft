#include "Chunk.h"

#include "Cube.h"
#include "util/blocks.h"

namespace sgl
{

    Chunk::Chunk(IVec3 pos) : position(pos)
    {
        blocks = new uint8_t[SIZE * SIZE * SIZE];
        memset(blocks, 0, SIZE * SIZE * SIZE);
    }

    Chunk::~Chunk()
    {
        delete[] blocks;
    }

    void Chunk::setBlock(IVec3 position, uint8_t type)
    {
        if (position.x >= 0 && position.x < SIZE && position.y >= 0 && position.y < SIZE && position.z >= 0 && position.z < SIZE)
        {
            blocks[position.x + SIZE * (position.y + SIZE * position.z)] = type;
        }
    }

    uint8_t Chunk::getBlock(IVec3 position) const
    {   
        if (!blocks || position.x < 0 || position.x >= SIZE || position.y < 0 || position.y >= SIZE || position.z < 0 || position.z >= SIZE)
            return 0;
        return blocks[position.x + SIZE * (position.y + SIZE * position.z)];
    }

    bool Chunk::isSolid(int x, int y, int z) const
    {
        return getBlock(IVec3(x, y, z)) != 0;
    }

    void Chunk::rebuildMesh()
    {
        mesh.clear();
        mesh.reserve(128);

        const int uvAxes[3][2] = {{1, 2}, {0, 2}, {0, 1}};
        const uint8_t faceLookup[3][2] = {{3, 2}, {5, 4}, {1, 0}};

        uint8_t mask[SIZE * SIZE];
        int x[3];

        for (int d = 0; d < 3; d++)
        {
            int u = uvAxes[d][0];
            int v = uvAxes[d][1];

            for (x[d] = 0; x[d] <= SIZE; ++x[d])
            {
                int n = 0;
                for (x[v] = 0; x[v] < SIZE; ++x[v])
                    for (x[u] = 0; x[u] < SIZE; ++x[u])
                    {
                        int prevCoord[3] = {x[0], x[1], x[2]};
                        prevCoord[d]--;

                        uint8_t bCurr = (x[d] < SIZE) ? getBlock(IVec3(x[0], x[1], x[2])) : 0;
                        uint8_t bPrev = (x[d] > 0) ? getBlock(IVec3(prevCoord[0], prevCoord[1], prevCoord[2])) : 0;

                        if (bPrev != 0 && bCurr == 0)
                            mask[n++] = 1;
                        else if (bPrev == 0 && bCurr != 0)
                            mask[n++] = 2;
                        else
                            mask[n++] = 0;
                    }

                n = 0;
                for (int j = 0; j < SIZE; ++j)
                    for (int i = 0; i < SIZE;)
                        if (mask[n] != 0)
                        {
                            int type = mask[n];
                            int w, h;

                            for (w = 1; i + w < SIZE && mask[n + w] == type; w++)
                            {
                            }

                            bool done = false;
                            for (h = 1; j + h < SIZE; h++)
                            {
                                for (int k = 0; k < w; k++)
                                {
                                    if (mask[n + k + h * SIZE] != type)
                                    {
                                        done = true;
                                        break;
                                    }
                                }
                                if (done)
                                    break;
                            }

                            x[u] = i;
                            x[v] = j;

                            int colorCoord[3] = {x[0], x[1], x[2]};
                            if (type == 1)
                                colorCoord[d]--;

                            uint8_t blockID = getBlock(IVec3(colorCoord[0], colorCoord[1], colorCoord[2]));
                            uint8_t faceIdx = faceLookup[d][type - 1];
                            uint8_t finalColor = getBlockColor(blockID, faceIdx);

                            mesh.push_back({(uint8_t)x[0], (uint8_t)x[1], (uint8_t)x[2],
                                            faceIdx,
                                            finalColor,
                                            (uint8_t)w, (uint8_t)h});

                            for (int ly = 0; ly < h; ly++)
                                for (int lx = 0; lx < w; lx++)
                                    mask[n + lx + ly * SIZE] = 0;

                            i += w;
                            n += w;
                        }
                        else
                        {
                            i++;
                            n++;
                        }
            }
        }
    }

    void Chunk::compress() 
    {
        if (blocks != nullptr) {
            delete[] blocks;
            blocks = nullptr;
        }
        mesh.shrink_to_fit();
    }
}