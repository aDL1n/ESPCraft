#include "Chunk.h"

namespace world
{

    Chunk::Chunk(sgl::IVec3 pos) : position(pos)
    {
        blocks = new uint8_t[SIZE * SIZE * SIZE];
        memset(blocks, 0, SIZE * SIZE * SIZE);

        mesh.position = pos * SIZE * 16;
    }

    Chunk::~Chunk()
    {
        delete[] blocks;
    }

    void Chunk::setBlock(sgl::IVec3 position, uint8_t type)
    {
        if (position.x >= 0 && position.x < SIZE && position.y >= 0 && position.y < SIZE && position.z >= 0 && position.z < SIZE)
            blocks[position.x + SIZE * (position.y + SIZE * position.z)] = type;
    }

    uint8_t Chunk::getBlock(sgl::IVec3 position) const
    {
        if (!blocks || position.x < 0 || position.x >= SIZE || position.y < 0 || position.y >= SIZE || position.z < 0 || position.z >= SIZE)
            return 0;

        return blocks[position.x + SIZE * (position.y + SIZE * position.z)];
    }

    void Chunk::rebuildMesh()
    {
        mesh.clear();
        mesh.reserve(128);

        const uint8_t uvAxes[3][2] = {{1, 2}, {0, 2}, {0, 1}};
        const uint8_t faceLookup[3][2] = {{3, 2}, {5, 4}, {1, 0}};

        uint8_t mask[SIZE * SIZE];
        uint8_t x[3];

        for (uint8_t d = 0; d < 3; d++)
        {
            uint8_t u = uvAxes[d][0];
            uint8_t v = uvAxes[d][1];

            for (x[d] = 0; x[d] <= SIZE; ++x[d])
            {
                uint16_t n = 0;
                for (x[v] = 0; x[v] < SIZE; ++x[v])
                    for (x[u] = 0; x[u] < SIZE; ++x[u])
                    {
                        uint8_t prevCoord[3] = {x[0], x[1], x[2]};
                        prevCoord[d]--;

                        uint8_t bCurr = (x[d] < SIZE) ? getBlock(sgl::IVec3(x[0], x[1], x[2])) : 0;
                        uint8_t bPrev = (x[d] > 0) ? getBlock(sgl::IVec3(prevCoord[0], prevCoord[1], prevCoord[2])) : 0;

                        if (bPrev != 0 && bCurr == 0)
                            mask[n++] = 1;
                        else if (bPrev == 0 && bCurr != 0)
                            mask[n++] = 2;
                        else
                            mask[n++] = 0;
                    }

                n = 0;
                for (uint8_t j = 0; j < SIZE; ++j)
                    for (uint8_t i = 0; i < SIZE;)
                        if (mask[n] != 0)
                        {
                            uint8_t type = mask[n];
                            uint8_t w, h;

                            for (w = 1; i + w < SIZE && mask[n + w] == type; w++)
                            {
                            }

                            bool done = false;
                            for (h = 1; j + h < SIZE; h++)
                            {
                                for (uint8_t k = 0; k < w; k++)
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

                            uint8_t colorCoord[3] = {x[0], x[1], x[2]};
                            if (type == 1)
                                colorCoord[d]--;

                            uint8_t blockID = getBlock(sgl::IVec3(colorCoord[0], colorCoord[1], colorCoord[2]));
                            uint8_t faceIdx = faceLookup[d][type - 1];
                            uint8_t finalColor = getBlockColor(blockID, faceIdx);

                            mesh.faces.push_back({x[0],
                                                  x[1],
                                                  x[2],
                                                  faceIdx,
                                                  finalColor,
                                                  w,
                                                  h});

                            for (uint8_t ly = 0; ly < h; ly++)
                                for (uint8_t lx = 0; lx < w; lx++)
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

        mesh.faces.shrink_to_fit();
    }
}