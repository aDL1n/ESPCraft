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

        sprite.createPalette(default_4bit_palette);
    }

    void Renderer::clear()
    {
        sprite.fillSprite(10);
        renderQueue.clear();
    }

    void Renderer::draw()
    {
        sprite.pushSprite(0, 0, 0);
    }

    TFT_eSprite &Renderer::getSprite()
    {
        return sprite;
    }

    void Renderer::drawMesh(const sgl::Mesh *mesh)
    {

        const float cx = mesh->position.x;
        const float cy = mesh->position.y;
        const float cz = mesh->position.z;

        const std::vector<MeshFace> &faces = mesh->faces;

        for (const MeshFace &face : faces)
        {
            uint8_t x = face.x;
            uint8_t y = face.y;
            uint8_t z = face.z;
            uint8_t w = face.w;
            uint8_t h = face.h;

            IVec3 v[4];
            switch (face.index)
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

            Vec3 projects[4];
            float avgZ = 0;
            bool visible = true;

            for (uint8_t i = 0; i < 4; i++)
            {
                projects[i] = camera->project({v[i].x + cx,
                                               v[i].y + cy,
                                               v[i].z + cz});

                if (projects[i].z < camera->near)
                {
                    visible = false;
                    break;
                }
                avgZ += projects[i].z;
            }

            if (!visible)
                continue;

            int32_t cross = (int32_t)(projects[1].x - projects[0].x) * (projects[2].y - projects[0].y) -
                            (int32_t)(projects[1].y - projects[0].y) * (projects[2].x - projects[0].x);

            if (cross >= 0)
                continue;

            renderQueue.push_back({(int16_t)projects[0].x, (int16_t)projects[0].y,
                                   (int16_t)projects[1].x, (int16_t)projects[1].y,
                                   (int16_t)projects[2].x, (int16_t)projects[2].y,
                                   (int16_t)projects[3].x, (int16_t)projects[3].y,
                                   (uint16_t)(avgZ * 0.25f),
                                   face.color});
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