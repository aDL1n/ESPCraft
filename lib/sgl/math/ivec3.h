#pragma once

#include <cstdint>

namespace sgl
{
    class IVec3
    {
    public:
        int32_t x, y, z;

        IVec3();
        IVec3(int32_t x, int32_t y, int32_t z);

        static IVec3 one();

        IVec3 operator+(const IVec3 &o) const;
        IVec3 operator+(int32_t i) const;
        IVec3 operator-(const IVec3 &o) const;
        IVec3 operator*(int32_t k) const;
    };
}