#pragma once

#include <cstdint>
#include <ArxContainer.h>

#include "math/vec3.h"

namespace sgl
{
    struct MeshFace
    {
        uint8_t x, y, z;
        uint8_t index;
        uint8_t color;
        uint8_t w, h;
    };

    struct Mesh
    {
        IVec3 position;
        std::vector<MeshFace> faces;

        void clear()
        {
            faces.clear();
        }

        void reserve(size_t capacity)
        {
            faces.reserve(capacity);
        }
    };
}