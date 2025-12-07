#include "vec3.h"

namespace sgl
{
    Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z)
    {
    }

    Vec3::Vec3() : x(0), y(0), z(0)
    {
    }

    Vec3 Vec3::operator+(const Vec3 &o) const
    {
        return Vec3(x + o.x, y + o.y, z + o.z);
    }

    Vec3 Vec3::operator+(const IVec3 &o) const
    {
        return Vec3(x + o.x, y + o.y, z + o.z);
    }

    Vec3 Vec3::operator+(float f) const
    {
        return Vec3(x + f, y + f, z + f);
    }

    Vec3 Vec3::operator-(const Vec3 &o) const
    {
        return Vec3(x - o.x, y - o.y, z - o.z);
    }

    Vec3 Vec3::operator*(float k) const
    {
        return Vec3(x * k, y * k, z * k);
    }

    bool Vec3::operator==(const Vec3 &y)
    {
        return this->x == y.x && this->y == y.y && this->z == y.z;
    }

    bool Vec3::operator==(const IVec3 &y)
    {
        return this->x == y.x && this->y == y.y && this->z == y.z;
    }
}