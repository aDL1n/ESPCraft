#pragma once

#include <math.h>
#include "vec3.h"
#include "ivec3.h"

namespace sgl
{
    class Mat3
    {
    public:
        float m[3][3];

        static Mat3 rotation(float ax, float ay, float az);
        static Mat3 rotation(Vec3 av);
        static Mat3 rotation(IVec3 av);

        Vec3 multiply(const Vec3 &v) const;
        Vec3 multiply(const IVec3 &v) const;
        Mat3 multiply(const Mat3 &o) const;
    };
}
