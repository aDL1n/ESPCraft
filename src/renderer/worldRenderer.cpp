#include "worldRenderer.h"

namespace renderer
{
    WorldRenderer::WorldRenderer(world::World &world, sgl::Renderer &sgl_renderer)
        : world(world), sgl_renderer(sgl_renderer)
    {
    }

    void WorldRenderer::render()
    {
        this->renderChunks();
    }

    void WorldRenderer::drawBlockOutline(sgl::IVec3 blockPosition)
    {
        
    }

    void WorldRenderer::setRenderDistance(uint8_t distance)
    {
        this->max_render_distance = distance;
    }

    void WorldRenderer::renderChunks()
    {
        for (world::Chunk *chunk : world.getChunks())
            if (chunk != nullptr)
                sgl_renderer.drawMesh(&chunk->getMesh());
    }
}