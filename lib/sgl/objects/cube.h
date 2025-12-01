#pragma once

#include <Arduino.h>
#include "math/vec3.h"

namespace sgl
{
    struct Face
    {
        uint32_t v1, v2, v3, v4;
        uint8_t color;
    };

    class Cube
    {
        public:

        static constexpr uint8_t vertices_size = 8;
        static constexpr uint8_t faces_size = 6;

        static const IVec3 vertices[vertices_size];
        static const Face faces[faces_size];
    };
}
