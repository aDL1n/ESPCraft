#pragma once

#include <TFT_eSPI.h>
#include <ArxContainer.h>

#include "math/mat3.h"
#include "objects/cube.h"
#include "objects/chunk.h"
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
        void drawChunk(const std::vector<Chunk*>& chunks);

        void draw();

    private:
        TFT_eSPI *tft;
        TFT_eSprite sprite;
        Camera *camera;

        bool debug;

        std::vector<RenderItem> renderQueue;

        const uint16_t palette[16] = {
            TFT_BLACK, TFT_ORANGE, TFT_DARKGREEN, TFT_DARKCYAN,
            TFT_MAROON, TFT_PURPLE, TFT_OLIVE, TFT_DARKGREY,
            TFT_BROWN, TFT_BLUE, TFT_GREEN, TFT_CYAN,
            TFT_RED, TFT_NAVY, TFT_YELLOW, TFT_WHITE};

        void drawFace(const Vec3 &p1, const Vec3 &p2, const Vec3 &p3, const Vec3 &p4, uint8_t colorIndex);
    };
}