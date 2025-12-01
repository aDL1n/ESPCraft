#pragma once

#include "math/vec3.h"
#include "math/mat3.h"

namespace sgl {

    class Camera {
    public:
        Vec3 position;
        Vec3 rotation;

        Mat3 viewMatrix;

        float fov;
        float near;
        float far;

        float focalLength;
        
        int16_t screenW;
        int16_t screenH;

        Camera(int16_t width = 320, int16_t height = 240, float fov = 75.0f);

        void update();

        Vec3 project(const Vec3& p) const;
    };
}