#pragma once

#include <TFT_eSPI.h>
#include <ArxContainer.h>

#include "math/mat3.h"
#include "objects/cube.h"
#include "objects/mesh.h"
#include "camera.h"

namespace sgl
{
    class Renderer
    {
    struct RenderItem {
        int16_t x1, y1, x2, y2, x3, y3, x4, y4;
        uint16_t zDepth;
        uint8_t color;
    };

    public:
        Renderer(TFT_eSPI *display, Camera *camera);

        void init(bool debug);
        void init();

        void clear();

        void drawCube(const Vec3 &av, const IVec3 &position);
        void drawMesh(const sgl::Mesh *mesh);

        void draw();

        TFT_eSprite* getSprite();
    private:
        TFT_eSPI *tft;
        TFT_eSprite sprite;
        Camera *camera;

        bool debug;

        std::vector<RenderItem> renderQueue;
    };
}