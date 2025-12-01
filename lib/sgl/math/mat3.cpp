#include "Mat3.h"

namespace sgl
{
    Mat3 Mat3::rotation(float ax, float ay, float az)
    {
        float cx = cos(ax), sx = sin(ax);
        float cy = cos(ay), sy = sin(ay);
        float cz = cos(az), sz = sin(az);

        Mat3 rx = {{{1, 0, 0},
                    {0, cx, -sx},
                    {0, sx, cx}}};

        Mat3 ry = {{{cy, 0, sy},
                    {0, 1, 0},
                    {-sy, 0, cy}}};

        Mat3 rz = {{{cz, -sz, 0},
                    {sz, cz, 0},
                    {0, 0, 1}}};

        return rz.multiply(ry).multiply(rx);
    }

    Mat3 Mat3::rotation(Vec3 av)
    {
        return Mat3::rotation(av.x, av.y, av.z);
    }

    Mat3 Mat3::rotation(IVec3 av)
    {
        return Mat3::rotation(av.x, av.y, av.z);
    }

    Vec3 Mat3::multiply(const Vec3 &v) const
    {
        return Vec3(
            v.x * m[0][0] + v.y * m[0][1] + v.z * m[0][2],
            v.x * m[1][0] + v.y * m[1][1] + v.z * m[1][2],
            v.x * m[2][0] + v.y * m[2][1] + v.z * m[2][2]);
    }
    
    Vec3 Mat3::multiply(const IVec3 &v) const
    {
        return Vec3(
            v.x * m[0][0] + v.y * m[0][1] + v.z * m[0][2],
            v.x * m[1][0] + v.y * m[1][1] + v.z * m[1][2],
            v.x * m[2][0] + v.y * m[2][1] + v.z * m[2][2]);
    }

    Mat3 Mat3::multiply(const Mat3 &o) const
    {
        Mat3 r;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                r.m[i][j] =
                    m[i][0] * o.m[0][j] +
                    m[i][1] * o.m[1][j] +
                    m[i][2] * o.m[2][j];
        return r;
    }
}