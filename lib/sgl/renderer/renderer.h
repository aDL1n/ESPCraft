#pragma once

#include <TFT_eSPI.h>
#include <ArxContainer.h>

#include "math/mat3.h"
#include "objects/cube.h"
#include "objects/mesh.h"
#include "objects/line.h"
#include "camera.h"

namespace sgl
{
    class Renderer
    {
    enum RenderType: bool
    {
        TRIANGLE=1,
        LINE=0
    };

    struct RenderItem {
        RenderType type;
        int16_t x1, y1, x2, y2, x3, y3;
        uint16_t zDepth;
        uint8_t color;
    };

    public:
        Renderer(TFT_eSPI *display, Camera *camera);

        void init(bool debug);
        void init();

        void clear();

        void drawMesh(const sgl::Mesh *mesh);
        void drawLine(Line line, Vec3 position);

        void draw();
        void display();

        TFT_eSprite& getSprite();
        TFT_eSPI& getDisplay();
    private:
        TFT_eSPI *tft;
        TFT_eSprite sprite;
        Camera *camera;

        bool debug;

        std::vector<RenderItem> renderQueue;
    };
}