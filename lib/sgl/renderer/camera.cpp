#include "Camera.h"
#include <math.h>
#include <Arduino.h>


namespace sgl
{

    Camera::Camera(int16_t width, int16_t height, float fov)
        : screenW(width), screenH(height), fov(fov), near(1.0f), far(1000.0f)
    {
        position = Vec3(0, 0, -50);
        rotation = Vec3(0, 0, 0);

        update();
    }

    void Camera::update()
    {
        focalLength = (screenW * 0.5f) / tanf((fov * 0.5f) * DEG_TO_RAD);

        float cosX = cosf(rotation.x);
        float sinX = sinf(rotation.x);
        float cosY = cosf(rotation.y);
        float sinY = sinf(rotation.y);
        float cosZ = cosf(rotation.z);
        float sinZ = sinf(rotation.z);

        viewMatrix.m[0][0] = cosY * cosZ;
        viewMatrix.m[0][1] = -cosX * sinZ + sinX * sinY * cosZ;
        viewMatrix.m[0][2] = sinX * sinZ + cosX * sinY * cosZ;

        viewMatrix.m[1][0] = cosY * sinZ;
        viewMatrix.m[1][1] = cosX * cosZ + sinX * sinY * sinZ;
        viewMatrix.m[1][2] = -sinX * cosZ + cosX * sinY * sinZ;

        viewMatrix.m[2][0] = -sinY;
        viewMatrix.m[2][1] = sinX * cosY;
        viewMatrix.m[2][2] = cosX * cosY;
    }

    Vec3 Camera::project(const Vec3 &p) const
    {
        float dx = p.x - position.x;
        float dy = p.y - position.y;
        float dz = p.z - position.z;

        float vx = viewMatrix.m[0][0] * dx + viewMatrix.m[1][0] * dy + viewMatrix.m[2][0] * dz;
        float vy = viewMatrix.m[0][1] * dx + viewMatrix.m[1][1] * dy + viewMatrix.m[2][1] * dz;
        float vz = viewMatrix.m[0][2] * dx + viewMatrix.m[1][2] * dy + viewMatrix.m[2][2] * dz;

        if (vz < near)
        {
            return Vec3(0, 0, vz);
        }

        float scale = focalLength / vz;

        return Vec3(
            vx * scale + (screenW * 0.5f),
            -vy * scale + (screenH * 0.5f),
            vz);
    }

    Vec3 Camera::getForward() const
    {
        return Vec3(
            viewMatrix.m[0][2],
            viewMatrix.m[1][2],
            viewMatrix.m[2][2]
        );
    }
}