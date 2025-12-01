#include "Renderer.h"
#include <algorithm>

namespace sgl
{
    Renderer::Renderer(TFT_eSPI *display, Camera *camera) : tft(display), sprite(display), camera(camera)
    {
        renderQueue.reserve(2048);
    }

    void Renderer::init()
    {
        this->init(false);
    }

    void Renderer::init(bool debug)
    {
        this->debug = debug;

        sprite.setColorDepth(4);
        sprite.createSprite(320, 240);

        sprite.createPalette(palette);
    }

    void Renderer::clear()
    {
        sprite.fillSprite(0);
        renderQueue.clear();
    }

    void Renderer::draw()
    {
        sprite.pushSprite(0, 0);
    }

    void Renderer::drawFace(const Vec3 &p1, const Vec3 &p2, const Vec3 &p3, const Vec3 &p4, uint8_t color)
    {
        sprite.createPalette(palette);

        if (debug)
        {
            sprite.drawTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color);
            sprite.drawTriangle(p1.x, p1.y, p3.x, p3.y, p4.x, p4.y, color);
        }
        else
        {
            sprite.fillTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color);
            sprite.fillTriangle(p1.x, p1.y, p3.x, p3.y, p4.x, p4.y, color);
        }
    }

    void Renderer::drawCube(const Vec3 &av, const IVec3 &position)
    {
        Mat3 rotation = Mat3::rotation(av);

        Vec3 transformed_vertices[Cube::vertices_size];
        for (int i = 0; i < Cube::vertices_size; ++i)
        {
            IVec3 local = Cube::vertices[i] * 16;
            transformed_vertices[i] = rotation.multiply(local) + position;
        }

        float face_z[Cube::faces_size];
        for (int i = 0; i < Cube::faces_size; ++i)
        {
            const Face &f = Cube::faces[i];
            face_z[i] = transformed_vertices[f.v1].z +
                        transformed_vertices[f.v2].z +
                        transformed_vertices[f.v3].z +
                        transformed_vertices[f.v4].z;
        }

        int order[Cube::faces_size] = {0, 1, 2, 3, 4, 5};
        std::sort(order, order + Cube::faces_size, [&](int16_t a, int16_t b)
                  { return face_z[a] > face_z[b]; });

        for (int ii = 0; ii < Cube::faces_size; ++ii)
        {
            const int i = order[ii];
            const Face &face = Cube::faces[i];

            Vec3 p1 = camera->project(transformed_vertices[face.v1]);
            Vec3 p2 = camera->project(transformed_vertices[face.v2]);
            Vec3 p3 = camera->project(transformed_vertices[face.v3]);
            Vec3 p4 = camera->project(transformed_vertices[face.v4]);

            int16_t v1x = p2.x - p1.x;
            int16_t v1y = p2.y - p1.y;
            int16_t v2x = p3.x - p1.x;
            int16_t v2y = p3.y - p1.y;
            int32_t cross = v1x * v2y - v1y * v2x;

            if (cross >= 0)
                continue;

            bool all_left = (p1.x < 0 && p2.x < 0 && p3.x < 0 && p4.x < 0);
            bool all_right = (p1.x > sprite.width() && p2.x > sprite.width() && p3.x > sprite.width() && p4.x > sprite.width());
            bool all_top = (p1.y < 0 && p2.y < 0 && p3.y < 0 && p4.y < 0);
            bool all_bottom = (p1.y > sprite.height() && p2.y > sprite.height() && p3.y > sprite.height() && p4.y > sprite.height());

            if (all_left || all_right || all_top || all_bottom)
                continue;

            drawFace(p1, p2, p3, p4, face.color);
        }
    }

    void Renderer::drawChunk(const std::vector<Chunk *> &chunks)
    {
        for (const Chunk *chunk : chunks)
        {
            const float cx = chunk->position.x * Chunk::SIZE * 16.0f;
            const float cy = chunk->position.y * Chunk::SIZE * 16.0f;
            const float cz = chunk->position.z * Chunk::SIZE * 16.0f;

            const std::vector<ChunkFace> &mesh = chunk->getMesh();

            for (const ChunkFace &f : mesh)
            {   
                float x = (float)f.x;
                float y = (float)f.y;
                float z = (float)f.z;
                float w = (float)f.w;
                float h = (float)f.h;

                Vec3 v[4];
                switch (f.faceIndex)
                {
                case 1:
                    v[0] = {x + w, y + h, z};
                    v[1] = {x + w, y, z};
                    v[2] = {x, y, z};
                    v[3] = {x, y + h, z};
                    break;
                case 0:
                    v[0] = {x, y + h, z};
                    v[1] = {x, y, z};
                    v[2] = {x + w, y, z};
                    v[3] = {x + w, y + h, z};
                    break;
                case 2:
                    v[0] = {x, y + w, z + h};
                    v[1] = {x, y, z + h};
                    v[2] = {x, y, z};
                    v[3] = {x, y + w, z};
                    break;
                case 3:
                    v[0] = {x, y + w, z};
                    v[1] = {x, y, z};
                    v[2] = {x, y, z + h};
                    v[3] = {x, y + w, z + h};
                    break;
                case 4:
                    v[0] = {x + w, y, z + h};
                    v[1] = {x + w, y, z};
                    v[2] = {x, y, z};
                    v[3] = {x, y, z + h};
                    break;
                case 5:
                    v[0] = {x, y, z + h};
                    v[1] = {x, y, z};
                    v[2] = {x + w, y, z};
                    v[3] = {x + w, y, z + h};
                    break;
                }

                Vec3 p[4];
                float avgZ = 0;
                bool visible = true;

                for (int i = 0; i < 4; i++)
                {
                    p[i] = camera->project({v[i].x * 16.0f + cx,
                                            v[i].y * 16.0f + cy,
                                            v[i].z * 16.0f + cz});

                    if (p[i].z < camera->near)
                    {
                        visible = false;
                        break;
                    }
                    avgZ += p[i].z;
                }

                if (!visible)
                    continue;

                int32_t cross = (int32_t)(p[1].x - p[0].x) * (p[2].y - p[0].y) -
                                (int32_t)(p[1].y - p[0].y) * (p[2].x - p[0].x);

                if (cross >= 0)
                    continue;

                renderQueue.push_back({(int16_t)p[0].x, (int16_t)p[0].y,
                                       (int16_t)p[1].x, (int16_t)p[1].y,
                                       (int16_t)p[2].x, (int16_t)p[2].y,
                                       (int16_t)p[3].x, (int16_t)p[3].y,
                                       (uint16_t)(avgZ * 0.25f),
                                       f.color});
            }
        }
        std::sort(renderQueue.begin(), renderQueue.end(), [](const RenderItem &a, const RenderItem &b)
                  { return a.zDepth > b.zDepth; });

        for (const RenderItem &item : renderQueue)
        {
            if (debug)
            {
                sprite.drawTriangle(item.x1, item.y1, item.x2, item.y2, item.x3, item.y3, item.color);
                sprite.drawTriangle(item.x1, item.y1, item.x3, item.y3, item.x4, item.y4, item.color);
            }
            else
            {
                sprite.fillTriangle(item.x1, item.y1, item.x2, item.y2, item.x3, item.y3, item.color);
                sprite.fillTriangle(item.x1, item.y1, item.x3, item.y3, item.x4, item.y4, item.color);
            }
        }
    }
}