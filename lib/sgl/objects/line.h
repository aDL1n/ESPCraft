#include <ArxContainer.h>

#include "math/ivec3.h"


namespace sgl
{
    struct Line
    {
        uint8_t x1, y1, z1, x2, y2, z2;
        uint8_t color;
    };

    struct LineMesh
    {
        IVec3 position;
        std::vector<Line> lines;

        void clear()
        {
            lines.clear();
        }

        void reserve(size_t capacity)
        {
            lines.reserve(capacity);
        }
    };
}