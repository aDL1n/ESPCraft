#pragma once

#include "ivec3.h"

namespace sgl
{
    class Vec3
    {
    public:
        float x, y, z;

        Vec3(float x, float y, float z);
        Vec3();

        Vec3 operator+(const Vec3 &o) const;
        Vec3 operator+(const sgl::IVec3 &o) const;
        Vec3 operator+(float f) const;
        Vec3 operator-(const Vec3 &o) const;
        Vec3 operator*(float k) const;
    };
}