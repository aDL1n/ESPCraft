#pragma once

#include <cstdint>
#include <sgl.h>

#include "world/world.h"

namespace renderer
{
    class WorldRenderer
    {
    public:
        explicit WorldRenderer(world::World &world, sgl::Renderer &sgl_renderer, sgl::Camera &camera);

        void render();
        void drawBlockOutline(sgl::IVec3 blockPosition);

        void setRenderDistance(uint8_t distance);
    private:
        world::World &world;
        sgl::Renderer &sgl_renderer;
        sgl::Camera &camera;

        uint8_t max_render_distance = 1; // Max render chunks

        void renderChunks();
    };
}