#include <TFT_eSPI.h>
#include <sgl.h>

#include "world/chunk.h"
#include "world/world.h"

using namespace sgl;

// TODO
// дописать vec2 и ivec2
// сделать вывод и рендер в разных ядрах есп (также как вариант 1 ядро графика, другое физика и логика)

// вот тут еще что то можно посмотреть https://github.com/EmberGL-org/EmberGL/

TFT_eSPI tft;
Camera camera(320, 240, 120);
Renderer *renderer;
Hud *hud;

world::World *w;

void setup()
{

    tft.init();
    tft.setRotation(1);

    camera.position = sgl::Vec3(0, 24, -16);
    camera.rotation.x = 40.0f * DEG_TO_RAD;
    camera.update();

    renderer = new Renderer(&tft, &camera);
    renderer->init(false);

    hud = new Hud(renderer->getSprite());

    w = new world::World();

    w->generate();
}

int16_t xi = -16;
uint8_t zi = 0;
uint8_t yi = 16;

bool place = false;

void render()
{   
    for (world::Chunk *chunk : w->getChunks())
        if (chunk != nullptr) 
            renderer->drawMesh(&chunk->getMesh());
    hud->render();

    renderer->draw();
    renderer->clear();
}

void update()
{   
    w->setBlock(place, IVec3(xi, yi - 1, zi));

    xi++;

    if (xi >= 16)
    {
        xi = -16;
        zi++;
    } 

    if (zi >= 16)
    {
        zi = 0;
        yi--;
    }

    if (yi <= 0)
    {
        yi = 16;
        place = !place;
    }
}

void loop()
{   
    render();
    update();
}