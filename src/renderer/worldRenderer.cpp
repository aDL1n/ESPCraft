#include "worldRenderer.h"

namespace renderer
{
    WorldRenderer::WorldRenderer(world::World &world, sgl::Renderer &sgl_renderer, sgl::Camera &camera)
        : world(world), sgl_renderer(sgl_renderer), camera(camera)
    {
    }

    void WorldRenderer::render()
    {
        this->renderChunks();
    }
    
    static const uint8_t cubeEdgeIndices[12][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0}, {4, 5}, {5, 6}, {6, 7}, {7, 4}, {0, 4}, {1, 5}, {2, 6}, {3, 7}};

    void WorldRenderer::drawBlockOutline(sgl::IVec3 blockPosition)
    {
        const float epsilon = 0.1f;
        const float minimumOffset = -epsilon;
        const float maximumOffset = 1.0f + epsilon;
        const uint8_t outlineColor = 9;

        sgl::Vec3 vertices[8];

        for (uint8_t vertexIndex = 0; vertexIndex < sgl::Cube::vertices_size; ++vertexIndex)
        {
            const sgl::IVec3 vertex = sgl::Cube::vertices[vertexIndex];

            vertices[vertexIndex] = {
                (vertex.x > 0) ? maximumOffset : minimumOffset,
                (vertex.y > 0) ? maximumOffset : minimumOffset,
                (vertex.z > 0) ? maximumOffset : minimumOffset};
        }

        for (uint8_t edgeIndex = 0; edgeIndex < 12; ++edgeIndex)
        {
            uint8_t firstVertexIndex = cubeEdgeIndices[edgeIndex][0];
            uint8_t secondVertexIndex = cubeEdgeIndices[edgeIndex][1];

            sgl::Line line{
                (uint8_t)vertices[firstVertexIndex].x,
                (uint8_t)vertices[firstVertexIndex].y,
                (uint8_t)vertices[firstVertexIndex].z,
                (uint8_t)vertices[secondVertexIndex].x,
                (uint8_t)vertices[secondVertexIndex].y,
                (uint8_t)vertices[secondVertexIndex].z,
                outlineColor};

            sgl_renderer.drawLine(
                line,
                {(float)blockPosition.x,
                 (float)blockPosition.y,
                 (float)blockPosition.z});
        }
    }

    void WorldRenderer::setRenderDistance(uint8_t distance)
    {
        this->max_render_distance = distance;
    }

    void WorldRenderer::renderChunks()
    {
        int32_t playerChunkX = (int32_t)camera.position.x >> 4;
        int32_t playerChunkZ = (int32_t)camera.position.z >> 4;

        for (world::Chunk *chunk : world.getChunks())
        {
            if (chunk == nullptr)
                continue;

            int32_t dx = chunk->position.x - playerChunkX;
            int32_t dz = chunk->position.z - playerChunkZ;

            if (dx < 0)
                dx = -dx;

            if (dx > max_render_distance)
                continue;

            if (dz < 0)
                dz = -dz;

            if (dz > max_render_distance)
                continue;

            sgl_renderer.drawMesh(&chunk->getMesh());
        }
    }
}