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

    void WorldRenderer::drawBlockOutline(sgl::IVec3 blockPosition)
    {
        const float epsilon = 0.1f;
        const float minimumOffset = -epsilon;
        const float maximumOffset = 1.0f + epsilon;
        const uint8_t outlineColor = 9;

        static const uint8_t cubeEdgeIndices[12][2] = {
            {0, 1}, {1, 2}, {2, 3}, {3, 0},
            {4, 5}, {5, 6}, {6, 7}, {7, 4},
            {0, 4}, {1, 5}, {2, 6}, {3, 7}
        };

        sgl::Vec3 projectedVertices[8];
        bool isVertexVisible[8];

        for (uint8_t vertexIndex = 0; vertexIndex < sgl::Cube::vertices_size; ++vertexIndex)
        {
            sgl::Vec3 worldPosition;
            const sgl::IVec3 cubeVertex = sgl::Cube::vertices[vertexIndex];

            worldPosition.x = (float)blockPosition.x + ((cubeVertex.x > 0) ? maximumOffset : minimumOffset);
            worldPosition.y = (float)blockPosition.y + ((cubeVertex.y > 0) ? maximumOffset : minimumOffset);
            worldPosition.z = (float)blockPosition.z + ((cubeVertex.z > 0) ? maximumOffset : minimumOffset);

            projectedVertices[vertexIndex] = camera.project(worldPosition);
            isVertexVisible[vertexIndex] = (projectedVertices[vertexIndex].z > camera.near);
        }

        for (uint8_t edgeIndex = 0; edgeIndex < 12; ++edgeIndex)
        {
            uint8_t firstVertexIndex = cubeEdgeIndices[edgeIndex][0];
            uint8_t secondVertexIndex = cubeEdgeIndices[edgeIndex][1];

            if (isVertexVisible[firstVertexIndex] && isVertexVisible[secondVertexIndex])
            {
                sgl_renderer.getSprite().drawLine(
                    projectedVertices[firstVertexIndex].x,
                    projectedVertices[firstVertexIndex].y,
                    projectedVertices[secondVertexIndex].x,
                    projectedVertices[secondVertexIndex].y,
                    outlineColor);
            }
        }
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