#include "ivec3.h"

namespace sgl
{
    IVec3::IVec3() : x(0), y(0), z(0)
    {
    }

    IVec3::IVec3(int32_t x, int32_t y, int32_t z) : x(x), y(y), z(z)
    {
    }

    IVec3 IVec3::one()
    {   
        return IVec3(1, 1, 1);
    }

    IVec3 IVec3::operator+(const IVec3 &o) const
    {
        return IVec3(x + o.x, y + o.y, z + o.z);
    }

    IVec3 IVec3::operator+(int32_t i) const
    {
        return IVec3(x + i, y + i, z + i);
    }

    IVec3 IVec3::operator-(const IVec3 &o) const
    {
        return IVec3(x - o.x, y - o.y, z - o.z);
    }

    IVec3 IVec3::operator*(int32_t k) const
    {
        return IVec3(x * k, y * k, z * k);
    }
}