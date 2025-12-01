#include "Cube.h"

namespace sgl
{
    const IVec3 Cube::vertices[Cube::vertices_size]
    {
        IVec3(-1, -1, -1),
        IVec3( 1, -1, -1),
        IVec3( 1,  1, -1),
        IVec3(-1,  1, -1),
        IVec3(-1, -1,  1),
        IVec3( 1, -1,  1),
        IVec3( 1,  1,  1),
        IVec3(-1,  1,  1)
    };

    constexpr Face Cube::faces[Cube::faces_size]
    {
        {0, 1, 2, 3, 12},
        {4, 7, 6, 5, 9},
        {0, 3, 7, 4, 15},
        {1, 5, 6, 2, 10},
        {0, 4, 5, 1, 14},
        {3, 2, 6, 7, 5}
    };
}
