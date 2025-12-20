#include <TFT_eSPI.h>
#include <sgl.h>
#include <scl.h>
#include <ArxContainer.h>

#include "world/chunk.h"
#include "world/world.h"

#include "renderer/worldRenderer.h"
#include "renderer/hudRenderer.h"

using namespace sgl;

// TODO
// дописать vec2 и ivec2
// сделать вывод и рендер в разных ядрах есп (также как вариант 1 ядро графика, другое физика и логика)

// вот тут еще что то можно посмотреть https://github.com/EmberGL-org/EmberGL/

TFT_eSPI tft;
Camera camera(320, 240, 90);
Renderer *sgl_renderer;

world::World *w;

renderer::WorldRenderer *worldRenderer;
renderer::HudRenderer *hudRenderer;

scl::Joystick *joystick;

void getRam()
{
    long free_heap_size = ESP.getFreeHeap();
    long largest_free_block = heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);

    Serial.print("Total free heap RAM: ");
    Serial.print(free_heap_size);
    Serial.println(" bytes");

    Serial.print("Largest free block: ");
    Serial.print(largest_free_block);
    Serial.println(" bytes");
}

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
    }

    tft.init();
    tft.setRotation(1);

    camera.position = sgl::Vec3(0, 18, 0);
    camera.rotation.x = 20 * DEG_TO_RAD;
    camera.update();

    joystick = new scl::Joystick(34, 35, -1, 1024);

    sgl_renderer = new Renderer(&tft, &camera);
    sgl_renderer->init();

    hudRenderer = new renderer::HudRenderer(sgl_renderer->getSprite());

    getRam();

    w = new world::World();
    w->generate();

    worldRenderer = new renderer::WorldRenderer(*w, *sgl_renderer, camera);

    getRam();
}

int16_t xi = -16;
int16_t zi = -16;
uint8_t yi = 16;

bool place = false;

void render()
{

    worldRenderer->render();

    world::BlockHit camara_block = w->getBlockAtView(camera.position, camera.getForward(), 8);
    if (camara_block.hit)
    {
        worldRenderer->drawBlockOutline(camara_block.position);
    }

    sgl_renderer->draw();

    hudRenderer->render();

    sgl_renderer->display();
    sgl_renderer->clear();
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
        zi = -16;
        yi--;
    }

    if (yi <= 0)
    {
        yi = 16;
        place = !place;
    }

    // joystick->read();

    // if (joystick->getX() == scl::State::UP)
    // {
    //     camera.rotation.x += 1.0f * DEG_TO_RAD;
    // }
    // else if (joystick->getX() == scl::State::DOWN)
    // {
    //     camera.rotation.x -= 1.0f * DEG_TO_RAD;
    // }

    // if (joystick->getY() == scl::State::UP)
    // {
    //     camera.rotation.y += 1.0f * DEG_TO_RAD;
    // }
    // else if (joystick->getY() == scl::State::DOWN)
    // {
    //     camera.rotation.y -= 1.0f * DEG_TO_RAD;
    // }

    camera.rotation.y += 0.5f * DEG_TO_RAD;
    camera.update();
}

void loop()
{
    render();
    update();
}